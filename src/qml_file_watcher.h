// src/qml_file_watcher.h
#ifndef QML_FILE_WATCHER_H
#define QML_FILE_WATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>

/**
 * QML文件监视器
 *
 * 职责：
 * - 监视文件系统变化（文件和目录，递归）
 * - 防抖逻辑（避免短时间内频繁触发）
 * - 发射文件变更信号
 */
class QmlFileWatcher : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlFileWatcher)

public:
    /**
     * 构造函数
     * @param parent     父对象
     * @param debounceMs 防抖延迟（毫秒），默认300ms
     */
    explicit QmlFileWatcher(QObject *parent = nullptr, int debounceMs = 300);

    /**
     * 递归监视目录中的所有QML文件
     * @param path 要监视的目录路径
     */
    void watchDirectory(const QString &path);

signals:
    /**
     * 文件变更信号（防抖后发射）
     * @param filePath 发生变化的文件或目录路径
     */
    void fileChanged(const QString &filePath);

private slots:
    void onFileSystemChanged(const QString &path);
    void onDebounceTimeout();

private:
    QFileSystemWatcher *watcher_;
    QTimer             *debounce_timer_;
    QString             last_changed_path_;
};

#endif // QML_FILE_WATCHER_H
