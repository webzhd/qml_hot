// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QDebug>
#include <QUrl>
#include <QQmlComponent>
#include <QMetaObject>
#include <QFileInfo>
#include <QDateTime>

QmlDynamicLoader::QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
{
    Q_ASSERT(engine_ != nullptr);
    qDebug() << "QmlDynamicLoader initialized";
}

bool QmlDynamicLoader::reloadLoader(QObject *loaderObj, const QString &qmlFilePath)
{
    if (!loaderObj) {
        qWarning() << "QmlDynamicLoader: Loader object is null";
        return false;
    }

    qDebug() << "[RELOAD] Starting reload for:" << qmlFilePath;

    // ===== 1. 验证文件存在 =====
    QFileInfo fileInfo(qmlFilePath);
    if (!fileInfo.exists()) {
        qWarning() << "[RELOAD] File does not exist:" << qmlFilePath;
        return false;
    }
    qDebug() << "[RELOAD] File exists, last modified:" << fileInfo.lastModified();

    // ===== 2. 清除所有缓存 =====
    engine_->clearComponentCache();
    qDebug() << "[RELOAD] Cache cleared";

    // ===== 关键：添加时间戳到 URL 以绕过缓存 =====
    QUrl url = QUrl::fromLocalFile(qmlFilePath);
    
    // 添加查询参数（时间戳）强制引擎认为这是一个新 URL
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    url.setQuery("reload=" + timestamp);
    
    qDebug() << "[RELOAD] Creating component with cache-busting URL:" << url.toString();

    // ===== 3. 创建新的 Component（带时间戳的 URL 会绕过缓存）=====
    QQmlComponent *component = new QQmlComponent(engine_, url, this);

    qDebug() << "[RELOAD] Component created, status:" << component->status();

    // ===== 4. 检查编译状态 =====
    if (component->status() == QQmlComponent::Error) {
        qWarning() << "[RELOAD] Component error:";
        for (const auto &error : component->errors()) {
            qWarning() << "  " << error.toString();
        }
        delete component;
        return false;
    }

    // ===== 5. 如果组件还在加载中（异步），等待加载完成 =====
    if (component->status() == QQmlComponent::Loading) {
        qDebug() << "[RELOAD] Component loading asynchronously, waiting...";
        pending_component_ = component;
        target_loader_ = loaderObj;
        
        connect(component, &QQmlComponent::statusChanged, this, 
            &QmlDynamicLoader::onLoaderStatusChanged, Qt::UniqueConnection);
        return true;
    }

    // ===== 6. 同步情况：直接设置 =====
    qDebug() << "[RELOAD] Component ready, setting immediately";
    
    // 清空旧的
    loaderObj->setProperty("sourceComponent", QVariant());
    
    // 设置新的
    QMetaObject::invokeMethod(this, [this, loaderObj, component]() {
        qDebug() << "[RELOAD] [QUEUED] Setting sourceComponent";
        loaderObj->setProperty("sourceComponent", QVariant::fromValue(component));
        qDebug() << "[RELOAD] [QUEUED] ✓ sourceComponent set";
    }, Qt::QueuedConnection);

    return true;
}

void QmlDynamicLoader::onLoaderStatusChanged()
{
    if (!pending_component_ || !target_loader_) {
        return;
    }

    qDebug() << "[RELOAD] [ASYNC] Component status changed:" << pending_component_->status();

    if (pending_component_->status() == QQmlComponent::Ready) {
        qDebug() << "[RELOAD] [ASYNC] Component ready!";
        
        // 清空旧的
        target_loader_->setProperty("sourceComponent", QVariant());
        
        // 设置新的
        QMetaObject::invokeMethod(this, [this]() {
            qDebug() << "[RELOAD] [ASYNC QUEUED] Setting pending component";
            target_loader_->setProperty("sourceComponent", 
                QVariant::fromValue(pending_component_));
            qDebug() << "[RELOAD] [ASYNC QUEUED] ✓ Set complete";
            
            pending_component_ = nullptr;
            target_loader_ = nullptr;
        }, Qt::QueuedConnection);
    } 
    else if (pending_component_->status() == QQmlComponent::Error) {
        qWarning() << "[RELOAD] [ASYNC] Component error:";
        for (const auto &error : pending_component_->errors()) {
            qWarning() << "  " << error.toString();
        }
        pending_component_->deleteLater();
        pending_component_ = nullptr;
        target_loader_ = nullptr;
    }
}