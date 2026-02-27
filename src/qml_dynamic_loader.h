// src/qml_dynamic_loader.h
#ifndef QML_DYNAMIC_LOADER_H
#define QML_DYNAMIC_LOADER_H

#include <QObject>
#include <QQmlApplicationEngine>  // 改这里！

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
     * @param engine QML应用引擎指针
     * @param parent 父对象
     */
    explicit QmlDynamicLoader(QQmlApplicationEngine *engine, QObject *parent = nullptr);  // 改这里！

    /**
     * 将QML文件重新加载到Loader对象
     *
     * @param loaderObj   目标Loader对象（需具有source属性）
     * @param qmlFilePath QML文件的绝对路径
     * @return 成功返回true，loaderObj为空时返回false
     */
    bool reloadLoader(QObject *loaderObj, const QString &qmlFilePath);

private:
    QQmlApplicationEngine *engine_;  // 改这里！
};

#endif // QML_DYNAMIC_LOADER_H