// src/qml_loader_registry.h
#ifndef QML_LOADER_REGISTRY_H
#define QML_LOADER_REGISTRY_H

#include <QMap>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>

class QmlDynamicLoader;

/**
 * QML Loader注册表
 *
 * 职责：
 * - 以objectName为键注册Loader与对应的QML文件路径
 * - 在需要热更新时，统一更新所有已注册的Loader
 * - 管理QmlDynamicLoader的生命周期
 */
class QmlLoaderRegistry : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlLoaderRegistry)

public:
    /**
     * 构造函数
     * @param engine QML应用引擎指针
     * @param parent 父对象
     */
    explicit QmlLoaderRegistry(QQmlApplicationEngine *engine,
                               QObject *parent = nullptr);

    /**
     * 注册Loader
     * @param loaderName  Loader对象的objectName
     * @param qmlFilePath 对应的QML文件绝对路径
     */
    void registerLoader(const QString &loaderName, const QString &qmlFilePath);

    /**
     * 清除QML组件缓存并重新加载所有已注册的Loader
     */
    void updateAll();

private:
    QObject *findLoader(const QString &loaderName) const;

    QQmlApplicationEngine        *engine_;
    QmlDynamicLoader             *dynamic_loader_;
    QMap<QString, QString>        loader_map_;   // objectName -> qmlFilePath
};

#endif // QML_LOADER_REGISTRY_H
