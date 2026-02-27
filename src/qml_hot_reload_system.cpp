// src/qml_hot_reload_system.cpp
#include "qml_hot_reload_system.h"
#include "qml_file_watcher.h"
#include "qml_loader_registry.h"
#include <QDebug>

QmlHotReloadSystem::QmlHotReloadSystem(QQmlApplicationEngine *engine,
                                       QObject *parent)
    : QObject(parent)
    , file_watcher_(new QmlFileWatcher(this))
    , loader_registry_(new QmlLoaderRegistry(engine, this))
{
    connect(file_watcher_, &QmlFileWatcher::fileChanged,
            this, &QmlHotReloadSystem::onFileChanged);

    qDebug() << "QmlHotReloadSystem initialized";
}

void QmlHotReloadSystem::watchDirectory(const QString &path)
{
    file_watcher_->watchDirectory(path);
}

void QmlHotReloadSystem::addLoader(const QString &loaderName,
                                   const QString &qmlFilePath)
{
    loader_registry_->registerLoader(loaderName, qmlFilePath);
}

void QmlHotReloadSystem::onFileChanged(const QString &filePath)
{
    qDebug() << "QmlHotReloadSystem: File changed:" << filePath;
    loader_registry_->updateAll();
    emit reloadCompleted();
}

void QmlHotReloadSystem::triggerInitialLoad()
{
    qDebug() << "QmlHotReloadSystem: Triggering initial load...";
    loader_registry_->updateAll();
}