// src/application.cpp
#include "application.h"
#include <QDebug>

Application* Application::create(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    
    static Application* instance = new Application();
    return instance;
}

Application::Application(QObject *parent)
    : QObject(parent), count_(0)
{
    qDebug() << "Application initialized";
}

void Application::increment()
{
    count_++;
    qDebug() << "Count incremented to:" << count_;
    emit countChanged();
}

void Application::decrement()
{
    if (count_ > 0) {
        count_--;
        qDebug() << "Count decremented to:" << count_;
        emit countChanged();
    }
}

void Application::reset()
{
    count_ = 0;
    qDebug() << "Count reset to 0";
    emit countChanged();
}

void Application::add(int value)
{
    count_ += value;
    qDebug() << "Count increased by" << value << "to:" << count_;
    emit countChanged();
}