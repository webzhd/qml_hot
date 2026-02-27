// src/qml_dynamic_loader.h
#ifndef QML_DYNAMIC_LOADER_H
#define QML_DYNAMIC_LOADER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>

/**
 * 动态QML加载器
 * 
 * 绕过QML引擎的URL缓存，直接从磁盘读取并编译QML文件
 */
class QmlDynamicLoader : public QObject
{
    Q_OBJECT
    
public:
    explicit QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    
    // 从磁盘加载QML文件内容并创建Component
    QQmlComponent* loadQmlFromFile(const QString &filePath);
    
private:
    QQmlApplicationEngine *engine_;
};

#endif