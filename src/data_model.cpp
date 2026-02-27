// src/data_model.cpp
#include "data_model.h"
#include <QDebug>
#include <QDateTime>

DataModel* DataModel::create(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    
    static DataModel* instance = new DataModel();
    return instance;
}

DataModel::DataModel(QObject *parent)
    : QObject(parent)
    , app_name_("Qt Quick Hot Reload Demo")
    , version_("1.0.0")
    , build_time_(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
{
    qDebug() << "DataModel initialized (singleton)";
    
    // ===== 初始化默认数据 =====
    items_ << "Item 1"
           << "Item 2"
           << "Item 3"
           << "Item 4"
           << "Item 5";
}

void DataModel::addItem(const QString &item)
{
    if (!item.isEmpty()) {
        items_.append(item);
        qDebug() << "Item added:" << item;
        emit itemsChanged();
    }
}

void DataModel::removeItem(int index)
{
    if (index >= 0 && index < items_.count()) {
        QString removed = items_.takeAt(index);
        qDebug() << "Item removed:" << removed;
        emit itemsChanged();
    }
}

void DataModel::clearItems()
{
    items_.clear();
    qDebug() << "All items cleared";
    emit itemsChanged();
}

QString DataModel::getItemAt(int index) const
{
    if (index >= 0 && index < items_.count()) {
        return items_.at(index);
    }
    return "";
}