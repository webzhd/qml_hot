// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QDebug>
#include <QUrl>
#include <QQmlComponent>
#include <QMetaObject>

QmlDynamicLoader::QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
    , last_component_(nullptr)
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

    qDebug() << "QmlDynamicLoader: Reloading" << loaderObj->objectName()
             << "with" << qmlFilePath;

    // ===== 步骤1：找到 mainWindow =====
    QObject *mainWindow = nullptr;
    for (QObject *root : engine_->rootObjects()) {
        if (root->objectName() == "mainWindow") {
            mainWindow = root;
            break;
        }
    }

    if (!mainWindow) {
        qWarning() << "QmlDynamicLoader: mainWindow not found!";
        return false;
    }
    qDebug() << "✓ Found mainWindow";

    // ===== 步骤2：清除旧 component（在 C++ 端管理生命周期）=====
    if (last_component_) {
        qDebug() << "QmlDynamicLoader: Deleting old component instance";
        last_component_->deleteLater();  // ← 在 C++ 端销毁，不要在 QML 中销毁
        last_component_ = nullptr;
    }

    // ===== 步骤3：清除组件缓存 =====
    qDebug() << "QmlDynamicLoader: Clearing component cache...";
    engine_->clearComponentCache();
    qDebug() << "✓ Component cache cleared";

    // ===== 步骤4：创建新 Component =====
    QUrl componentUrl = QUrl::fromLocalFile(qmlFilePath);
    qDebug() << "QmlDynamicLoader: Creating component from:" << componentUrl;
    
    QQmlComponent *component = new QQmlComponent(engine_, componentUrl, this);
    last_component_ = component;  // ← 保存引用以便后续删除

    // ===== 步骤5：处理同步加载 =====
    if (component->status() == QQmlComponent::Ready) {
        qDebug() << "QmlDynamicLoader: Component ready (synchronous)";
        return invokeSetComponent(mainWindow, component);
    } 
    // ===== 步骤6：处理异步加载 =====
    else if (component->status() == QQmlComponent::Loading) {
        qDebug() << "QmlDynamicLoader: Component loading (asynchronous)";
        
        connect(component, &QQmlComponent::statusChanged, this, 
            [this, mainWindow, component]() {
                if (component->status() == QQmlComponent::Ready) {
                    qDebug() << "QmlDynamicLoader: Async component ready";
                    invokeSetComponent(mainWindow, component);
                } else if (component->status() == QQmlComponent::Error) {
                    qWarning() << "QmlDynamicLoader: Async component error:";
                    for (const auto &error : component->errors()) {
                        qWarning() << "  " << error.toString();
                    }
                    component->deleteLater();
                }
            }, Qt::SingleShotConnection);
        
        return true;
    }
    // ===== 步骤7：处理加载错误 =====
    else if (component->status() == QQmlComponent::Error) {
        qWarning() << "QmlDynamicLoader: Component error:";
        for (const auto &error : component->errors()) {
            qWarning() << "  " << error.toString();
        }
        component->deleteLater();
        last_component_ = nullptr;
        return false;
    }

    qWarning() << "QmlDynamicLoader: Unexpected component status:" << component->status();
    component->deleteLater();
    last_component_ = nullptr;
    return false;
}

bool QmlDynamicLoader::invokeSetComponent(QObject *mainWindow, QQmlComponent *component)
{
    if (!mainWindow || !component) {
        qWarning() << "QmlDynamicLoader::invokeSetComponent: Invalid parameters";
        return false;
    }

    qDebug() << "QmlDynamicLoader: Invoking mainWindow.setContentComponent()";
    
    bool success = QMetaObject::invokeMethod(mainWindow, "setContentComponent",
        Q_ARG(QVariant, QVariant::fromValue(component)));

    if (!success) {
        qWarning() << "QmlDynamicLoader: Failed to invoke setContentComponent!";
        component->deleteLater();
        last_component_ = nullptr;
        return false;
    }

    qDebug() << "✓ setContentComponent invoked successfully";
    return true;
}