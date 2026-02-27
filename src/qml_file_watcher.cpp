// src/qml_file_watcher.cpp
#include "qml_file_watcher.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>

QmlFileWatcher::QmlFileWatcher(QObject *parent, int debounceMs)
    : QObject(parent)
    , watcher_(new QFileSystemWatcher(this))
    , debounce_timer_(new QTimer(this))
{
    debounce_timer_->setInterval(debounceMs);
    debounce_timer_->setSingleShot(true);

    connect(watcher_, &QFileSystemWatcher::fileChanged,
            this, &QmlFileWatcher::onFileSystemChanged);
    connect(watcher_, &QFileSystemWatcher::directoryChanged,
            this, &QmlFileWatcher::onFileSystemChanged);
    connect(debounce_timer_, &QTimer::timeout,
            this, &QmlFileWatcher::onDebounceTimeout);

    qDebug() << "QmlFileWatcher initialized";
}

void QmlFileWatcher::watchDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "QmlFileWatcher: Directory does not exist:" << path;
        return;
    }

    if (!watcher_->addPath(path)) {
        qWarning() << "QmlFileWatcher: Failed to watch directory:" << path;
        return;
    }

    qDebug() << "QmlFileWatcher: Watching directory:" << path;

    const QFileInfoList fileList = dir.entryInfoList({"*.qml"}, QDir::Files);
    for (const QFileInfo &fileInfo : fileList) {
        watcher_->addPath(fileInfo.absoluteFilePath());
        qDebug() << "  Watching file:" << fileInfo.fileName();
    }

    const QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &dirInfo : dirList) {
        watchDirectory(dirInfo.absoluteFilePath());
    }
}

void QmlFileWatcher::onFileSystemChanged(const QString &path)
{
    QFileInfo info(path);
    if (info.isDir()) {
        // Re-scan directory so any newly added .qml files get watched
        watchDirectory(path);
        last_changed_path_ = path;
        debounce_timer_->stop();
        debounce_timer_->start();
    } else if (path.endsWith(".qml")) {
        qDebug() << "QmlFileWatcher: Change detected:" << path;
        last_changed_path_ = path;
        debounce_timer_->stop();
        debounce_timer_->start();
    }
}

void QmlFileWatcher::onDebounceTimeout()
{
    emit fileChanged(last_changed_path_);
}
