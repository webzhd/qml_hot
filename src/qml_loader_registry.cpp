// src/qml_loader_registry.cpp
#include "qml_loader_registry.h"
#include "qml_dynamic_loader.h"
#include <QDebug>

QmlLoaderRegistry::QmlLoaderRegistry(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
    , dynamic_loader_(new QmlDynamicLoader(engine, this))
{
    Q_ASSERT(engine_ != nullptr);
    qDebug() << "QmlLoaderRegistry initialized";
}

void QmlLoaderRegistry::registerLoader(const QString &loaderName,
                                       const QString &qmlFilePath)
{
    loader_map_[loaderName] = qmlFilePath;
    qDebug() << "QmlLoaderRegistry: Registered loader"
             << loaderName << "->" << qmlFilePath;
}

void QmlLoaderRegistry::updateAll()
{
    qDebug() << "QmlLoaderRegistry: Updating all loaders...";

    for (auto it = loader_map_.constBegin(); it != loader_map_.constEnd(); ++it) {
        QObject *loaderObj = findLoader(it.key());
        if (!loaderObj) {
            qWarning() << "QmlLoaderRegistry: Loader not found:" << it.key();
            continue;
        }
        qDebug() << "QmlLoaderRegistry: Reloading" << it.key() << "->" << it.value();
        dynamic_loader_->reloadLoader(loaderObj, it.value());
    }

    qDebug() << "QmlLoaderRegistry: All loaders updated";
}

QObject *QmlLoaderRegistry::findLoader(const QString &loaderName) const
{
    for (QObject *root : engine_->rootObjects()) {
        QObject *found = root->findChild<QObject *>(loaderName);
        if (found) {
            return found;
        }
    }
    return nullptr;
}
