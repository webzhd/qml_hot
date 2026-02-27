// src/qml_reloader_signal.h
#ifndef QML_RELOADER_SIGNAL_H
#define QML_RELOADER_SIGNAL_H

#include <QObject>

/**
 * QML重载信号发送器
 * 用于在C++和QML之间通信
 */
class QmlReloaderSignal : public QObject
{
    Q_OBJECT
    
public:
    explicit QmlReloaderSignal(QObject *parent = nullptr)
        : QObject(parent) {}
    
signals:
    void reloadRequested();
    
public slots:
    void requestReload() {
        emit reloadRequested();
    }
};

#endif