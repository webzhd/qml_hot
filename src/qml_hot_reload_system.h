// src/qml_hot_reload_system.h
#ifndef QML_HOT_RELOAD_SYSTEM_H
#define QML_HOT_RELOAD_SYSTEM_H

#include <QObject>
#include <QQmlApplicationEngine>

class QmlFileWatcher;
class QmlLoaderRegistry;

class QmlHotReloadSystem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlHotReloadSystem)

public:
    explicit QmlHotReloadSystem(QQmlApplicationEngine *engine,
                                QObject *parent = nullptr);

    void watchDirectory(const QString &path);
    void addLoader(const QString &loaderName, const QString &qmlFilePath);

signals:
    void reloadCompleted();

private slots:
    void onFileChanged(const QString &filePath);
    void triggerInitialLoad();  // 新增

private:
    QmlFileWatcher    *file_watcher_;
    QmlLoaderRegistry *loader_registry_;
};

#endif // QML_HOT_RELOAD_SYSTEM_H