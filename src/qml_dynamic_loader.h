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
    QQmlApplicationEngine *engine_;
};

#endif