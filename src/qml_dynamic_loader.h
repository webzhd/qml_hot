// src/qml_dynamic_loader.h
#ifndef QML_DYNAMIC_LOADER_H
#define QML_DYNAMIC_LOADER_H

#include <QObject>
#include <QQmlApplicationEngine>

class QQmlComponent;

class QmlDynamicLoader : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlDynamicLoader)

public:
    explicit QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    bool reloadLoader(QObject *loaderObj, const QString &qmlFilePath);

private:
    bool invokeSetComponent(QObject *mainWindow, QQmlComponent *component);

    QQmlApplicationEngine *engine_;
    QQmlComponent *last_component_;  // ← 新增：保存上一个 component 以便销毁
};

#endif // QML_DYNAMIC_LOADER_H