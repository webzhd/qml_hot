// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QFile>
#include <QDebug>
#include <QQmlContext>

QmlDynamicLoader::QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
{
}

QQmlComponent* QmlDynamicLoader::loadQmlFromFile(const QString &filePath)
{
    // ===== 从磁盘读取QML文件 =====
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return nullptr;
    }
    
    QString qmlContent = QString::fromUtf8(file.readAll());
    file.close();
    
    qDebug() << "Read QML file:" << filePath;
    qDebug() << "Content size:" << qmlContent.length() << "bytes";
    
    // ===== 从字符串创建Component（绕过URL缓存） =====
    // 这是关键：使用QQmlComponent的构造函数，传入QML代码字符串
    QQmlComponent *component = new QQmlComponent(engine_, this);
    component->setData(qmlContent.toUtf8(), QUrl::fromLocalFile(filePath));
    
    // ===== 检查编译错误 =====
    if (component->isError()) {
        qWarning() << "QML compilation errors:";
        for (const auto &error : component->errors()) {
            qWarning() << error.toString();
        }
        delete component;
        return nullptr;
    }
    
    qDebug() << "✓ QML component compiled successfully";
    return component;
}