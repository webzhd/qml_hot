// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QDebug>
#include <QUrl>
#include <QQmlComponent>
#include <QMetaObject>
#include <QTimer>

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

    qDebug() << "QmlDynamicLoader: Reloading" << loaderObj->objectName()
             << "with" << qmlFilePath;

    // ===== 获取 mainWindow =====
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

    // ===== 清除缓存 =====
    engine_->clearComponentCache();
    qDebug() << "QmlDynamicLoader: Component cache cleared";

    // ===== 设置 contentViewPath 属性（关键！）=====
    // 将完整路径传给 QML，这样 QML 可以正确加载
    bool pathSet = mainWindow->setProperty("contentViewPath", QUrl::fromLocalFile(qmlFilePath).toString());
    if (!pathSet) {
        qWarning() << "QmlDynamicLoader: Failed to set contentViewPath property!";
        return false;
    }
    qDebug() << "QmlDynamicLoader: contentViewPath set to:" << qmlFilePath;

    // ===== 调用 QML 中的 reloadContent() 方法 =====
    qDebug() << "QmlDynamicLoader: Calling mainWindow.reloadContent()...";
    bool success = QMetaObject::invokeMethod(mainWindow, "reloadContent");
    
    if (!success) {
        qWarning() << "QmlDynamicLoader: Failed to invoke reloadContent method!";
        return false;
    }

    qDebug() << "✓ QML reload triggered successfully";
    return true;
}