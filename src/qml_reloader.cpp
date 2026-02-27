// src/qml_reloader.cpp
#include "qml_reloader.h"
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QQmlComponent>

QmlReloader::QmlReloader(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
    , watcher_(new QFileSystemWatcher(this))
    , debounce_timer_(new QTimer(this))
    , content_component_(nullptr)
{
    Q_ASSERT(engine_ != nullptr);
    
    debounce_timer_->setInterval(300);
    debounce_timer_->setSingleShot(true);
    
    connect(watcher_, &QFileSystemWatcher::fileChanged,
            this, &QmlReloader::on_file_changed);
    
    connect(watcher_, &QFileSystemWatcher::directoryChanged,
            this, &QmlReloader::on_file_changed);
    
    connect(debounce_timer_, &QTimer::timeout,
            this, &QmlReloader::do_reload);
    
    qDebug() << "QmlReloader initialized";
}

void QmlReloader::watch_directory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return;
    }
    
    if (!watcher_->addPath(path)) {
        qWarning() << "Failed to watch directory:" << path;
        return;
    }
    
    qDebug() << "Watching directory:" << path;
    
    QStringList nameFilters;
    nameFilters << "*.qml";
    
    QFileInfoList fileList = dir.entryInfoList(nameFilters, QDir::Files);
    for (const QFileInfo &fileInfo : fileList) {
        QString absolutePath = fileInfo.absoluteFilePath();
        watcher_->addPath(absolutePath);
        qDebug() << "  Watching file:" << fileInfo.fileName();
    }
    
    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &dirInfo : dirList) {
        watch_directory(dirInfo.absoluteFilePath());
    }
}

void QmlReloader::on_file_changed(const QString &filePath)
{
    if (!filePath.endsWith(".qml")) {
        return;
    }
    
    qDebug() << "QML file changed:" << filePath;
    emit qml_file_changed(filePath);
    
    debounce_timer_->stop();
    debounce_timer_->start();
}

void QmlReloader::do_reload()
{
    qDebug() << "========== Reloading QML ==========";
    
    emit reload_started();
    
    try {
        qDebug() << "Clearing component cache...";
        engine_->clearComponentCache();
        
        reload_with_component();
        
    } catch (const std::exception &e) {
        qWarning() << "Exception during reload:" << e.what();
    }
}

void QmlReloader::reload_with_component()
{
    qDebug() << "Triggering QML reload...";
    
    auto rootObjects = engine_->rootObjects();
    if (rootObjects.isEmpty()) {
        qWarning() << "No root objects found!";
        return;
    }
    
    // ===== 查找mainWindow =====
    QObject *mainWindow = nullptr;
    for (auto obj : rootObjects) {
        if (obj->objectName() == "mainWindow") {
            mainWindow = obj;
            break;
        }
    }
    
    if (!mainWindow) {
        qWarning() << "mainWindow not found!";
        return;
    }
    
    qDebug() << "✓ Found mainWindow";
    
    // ===== 调用QML中的reloadContent函数 =====
    qDebug() << "Calling mainWindow.reloadContent()...";
    QMetaObject::invokeMethod(mainWindow, "reloadContent");
    
    qDebug() << "✓ QML reload triggered";
    emit reload_finished();
    qDebug() << "===================================";
}

void QmlReloader::full_reload()
{
    qDebug() << "⚠ Performing full reload...";
    
    auto rootObjects = engine_->rootObjects();
    for (auto obj : rootObjects) {
        if (obj) {
            obj->deleteLater();
        }
    }
    
    if (!main_qml_file_.isEmpty()) {
        engine_->load(main_qml_file_);
        qDebug() << "✓ Full reload completed";
    }
    
    emit reload_finished();
}