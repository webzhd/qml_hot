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

    // Reset source to empty first so the Loader registers a true change
    loaderObj->setProperty("source", QVariant(QUrl()));
    loaderObj->setProperty("source", QUrl::fromLocalFile(qmlFilePath));

    qDebug() << "QmlDynamicLoader: Reload triggered for" << loaderObj->objectName();
    return true;
}
