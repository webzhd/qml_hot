// src/qml_reloader.h
#ifndef QML_RELOADER_H
#define QML_RELOADER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QFileSystemWatcher>
#include <QUrl>
#include <QTimer>
#include <QQmlComponent>

/**
 * QML热重载器
 * 
 * 监视QML文件变化，自动重新加载
 * 支持：
 * - 文件系统变化检测
 * - 自动QML重新加载
 * - 防抖动（避免频繁重新加载）
 * - 调试日志
 */
class QmlReloader : public QObject
{
    Q_OBJECT
    
    // ===== 禁止复制和移动 =====
    Q_DISABLE_COPY_MOVE(QmlReloader)
    
public:
    /**
     * 构造函数
     * @param engine QML应用引擎指针
     * @param parent 父对象
     */
    explicit QmlReloader(QQmlApplicationEngine *engine, 
                        QObject *parent = nullptr);
    
    /**
     * 监视目录
     * 添加目录到文件监视列表
     * @param path 要监视的目录路径
     */
    void watch_directory(const QString &path);
    
    /**
     * 获取主QML文件URL
     * @return 当前加载的QML文件URL
     */
    QUrl get_main_qml_file() const { return main_qml_file_; }
    
    /**
     * 设置主QML文件
     * @param url QML文件路径
     */
    void set_main_qml_file(const QUrl &url) { main_qml_file_ = url; }
    
signals:
    /**
     * QML文件改变信号
     */
    void qml_file_changed(const QString &filePath);
    
    /**
     * 重新加载开始信号
     */
    void reload_started();
    
    /**
     * 重新加载完成信号
     */
    void reload_finished();

private slots:
    /**
     * 文件改变槽函数
     */
    void on_file_changed(const QString &filePath);
    
    /**
     * 执行重新加载的槽函数
     */
    void do_reload();

private:
    void reload_with_component();
    void full_reload();
    
    // ===== 成员变量 =====
    QQmlApplicationEngine *engine_;
    QFileSystemWatcher *watcher_;
    QTimer *debounce_timer_;
    QUrl main_qml_file_;
    QString content_qml_file_;
    QQmlComponent *content_component_;
};

#endif // QML_RELOADER_H