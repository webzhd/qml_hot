// src/qml_hot_reload_system.h
#ifndef QML_HOT_RELOAD_SYSTEM_H
#define QML_HOT_RELOAD_SYSTEM_H

#include <QObject>
#include <QQmlApplicationEngine>

class QmlFileWatcher;
class QmlLoaderRegistry;

/**
 * QML热更新系统（门面类）
 *
 * 整合文件监视（QmlFileWatcher）、动态加载（QmlDynamicLoader）和
 * Loader注册表（QmlLoaderRegistry），提供简洁的热更新接口。
 *
 * 使用示例：
 * @code
 * QmlHotReloadSystem reloadSystem(&engine);
 * reloadSystem.watchDirectory(qmlSourceDir);
 * reloadSystem.addLoader("contentLoader", qmlSourceDir + "/ContentView.qml");
 * @endcode
 */
class QmlHotReloadSystem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlHotReloadSystem)

public:
    /**
     * 构造函数
     * @param engine QML应用引擎指针
     * @param parent 父对象
     */
    explicit QmlHotReloadSystem(QQmlApplicationEngine *engine,
                                QObject *parent = nullptr);

    /**
     * 递归监视目录中的所有QML文件变化
     * @param path 要监视的目录路径
     */
    void watchDirectory(const QString &path);

    /**
     * 注册需要热更新的Loader
     * @param loaderName  Loader的objectName
     * @param qmlFilePath 对应的QML文件绝对路径
     */
    void addLoader(const QString &loaderName, const QString &qmlFilePath);

signals:
    /**
     * 所有Loader完成热更新后发射
     */
    void reloadCompleted();

private slots:
    void onFileChanged(const QString &filePath);

private:
    QmlFileWatcher    *file_watcher_;
    QmlLoaderRegistry *loader_registry_;
};

#endif // QML_HOT_RELOAD_SYSTEM_H
