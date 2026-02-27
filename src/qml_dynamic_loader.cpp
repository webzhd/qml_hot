// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QDebug>
#include <QUrl>
#include <QQmlComponent>
#include <QMetaObject>

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

    // ===== 1. 清除所有缓存 =====
    engine_->clearComponentCache();
    qDebug() << "[RELOAD] Cache cleared";

    // ===== 2. 创建新的 Component（这会从磁盘读取最新文件）=====
    QUrl url = QUrl::fromLocalFile(qmlFilePath);
    QQmlComponent *component = new QQmlComponent(engine_, url, this);

    qDebug() << "[RELOAD] Component created, status:" << component->status();

    // ===== 3. 检查编译状态 =====
    if (component->status() == QQmlComponent::Error) {
        qWarning() << "[RELOAD] Component error:";
        for (const auto &error : component->errors()) {
            qWarning() << "  " << error.toString();
        }
        delete component;
        return false;
    }

    // ===== 4. 为了安全，先清空 sourceComponent =====
    loaderObj->setProperty("sourceComponent", QVariant());
    qDebug() << "[RELOAD] Loader sourceComponent cleared";

    // ===== 5. 设置新的 sourceComponent（同步替换）=====
    bool success = loaderObj->setProperty("sourceComponent", QVariant::fromValue(component));
    
    if (!success) {
        qWarning() << "[RELOAD] Failed to set sourceComponent!";
        delete component;
        return false;
    }

    qDebug() << "[RELOAD] ✓ sourceComponent set successfully";
    qDebug() << "[RELOAD] Item status:" << loaderObj->property("status").toInt();

    return true;
}