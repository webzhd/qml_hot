// src/qml_dynamic_loader.h
#ifndef QML_DYNAMIC_LOADER_H
#define QML_DYNAMIC_LOADER_H

#include <QObject>
#include <QQmlEngine>

/**
 * QML动态加载器
 *
 * 职责：
 * - 清除QML组件缓存
 * - 将磁盘上的QML文件重新载入指定的Loader对象
 */
class QmlDynamicLoader : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QmlDynamicLoader)

public:
    /**
     * 构造函数
     * @param engine QML引擎指针
     * @param parent 父对象
     */
    explicit QmlDynamicLoader(QQmlEngine *engine, QObject *parent = nullptr);

    /**
     * 将QML文件重新加载到Loader对象
     *
     * 先将Loader的source清空，再设为新URL，
     * 以确保即使路径相同也会触发重新加载。
     *
     * @param loaderObj   目标Loader对象（需具有source属性）
     * @param qmlFilePath QML文件的绝对路径
     * @return 成功返回true，loaderObj为空时返回false
     */
    bool reloadLoader(QObject *loaderObj, const QString &qmlFilePath);

private:
    QQmlEngine *engine_;
};

#endif // QML_DYNAMIC_LOADER_H
