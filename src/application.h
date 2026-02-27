// src/application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QtQml>  // 必须

/**
 * 应用逻辑类
 */
class Application : public QObject
{
    Q_OBJECT
    
    // ===== 自动注册到QML =====
    QML_ELEMENT
    QML_SINGLETON
    
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    
public:
    Q_DISABLE_COPY_MOVE(Application)
    
    // 单例工厂方法
    static Application* create(QQmlEngine *engine, QJSEngine *scriptEngine);
    
    explicit Application(QObject *parent = nullptr);
    
    int count() const { return count_; }
    
    Q_INVOKABLE void increment();
    Q_INVOKABLE void decrement();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void add(int value);
    
signals:
    void countChanged();
    
private:
    int count_ = 0;
};

#endif // APPLICATION_H