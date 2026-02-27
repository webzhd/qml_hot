// src/qml_reloader.h
#ifndef QML_RELOADER_H
#define QML_RELOADER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QUrl>

class QmlDynamicLoader;

class QmlReloader : public QObject
{
    Q_OBJECT
    
public:
    explicit QmlReloader(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    
    void set_main_qml_file(const QUrl &file) { main_qml_file_ = file; }
    void watch_directory(const QString &path);
    
    // ===== 加载初始内容 =====
    void loadInitialContent(const QString &contentViewPath);
    
signals:
    void reload_started();
    void reload_finished();
    void qml_file_changed(const QString &filePath);
    
private slots:
    void on_file_changed(const QString &filePath);
    void do_reload();
    void reload_with_component();
    void full_reload();
    
private:
    // ===== 公共重载方法 =====
    void performReload(const QString &contentViewPath);
    
    QQmlApplicationEngine *engine_;
    QFileSystemWatcher *watcher_;
    QTimer *debounce_timer_;
    QUrl main_qml_file_;
    QmlDynamicLoader *dynamic_loader_;
};

#endif