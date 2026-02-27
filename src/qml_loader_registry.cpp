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
    qDebug() << "\n==========================================";
    qDebug() << "[UPDATE] QmlLoaderRegistry: Updating all loaders...";
    qDebug() << "==========================================";

    for (auto it = loader_map_.constBegin(); it != loader_map_.constEnd(); ++it) {
        QObject *loaderObj = findLoader(it.key());
        if (!loaderObj) {
            qWarning() << "[UPDATE] Loader not found:" << it.key();
            continue;
        }
        qDebug() << "[UPDATE] Calling reloadLoader for:" << it.key();
        dynamic_loader_->reloadLoader(loaderObj, it.value());
    }

    qDebug() << "[UPDATE] All loaders updated";
    qDebug() << "==========================================\n";
}

QObject *QmlLoaderRegistry::findLoader(const QString &loaderName) const
{
    for (QObject *root : engine_->rootObjects()) {
        QObject *found = root->findChild<QObject *>(loaderName);
        if (found) {
            qDebug() << "[FIND] Found loader:" << loaderName;
            return found;
        }
    }
    qWarning() << "[FIND] Loader not found:" << loaderName;
    return nullptr;
}