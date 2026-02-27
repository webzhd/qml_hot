// src/qml_dynamic_loader.cpp
#include "qml_dynamic_loader.h"
#include <QDebug>
#include <QUrl>

QmlDynamicLoader::QmlDynamicLoader(QQmlEngine *engine, QObject *parent)
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

    // Reset source to empty first so the cache can be cleared cleanly
    loaderObj->setProperty("source", QVariant(QUrl()));

    // Clear component cache after unloading the old component
    engine_->clearComponentCache();
    qDebug() << "QmlDynamicLoader: Component cache cleared";

    // Set new source to reload from disk
    const QUrl newUrl = QUrl::fromLocalFile(qmlFilePath);
    loaderObj->setProperty("source", newUrl);

    qDebug() << "QmlDynamicLoader: Reload triggered for" << loaderObj->objectName()
             << "source:" << newUrl;
    return true;
}
