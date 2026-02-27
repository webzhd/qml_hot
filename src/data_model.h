// src/data_model.h
#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <QObject>
#include <QString>
#include <QtQml>

class DataModel : public QObject
{
    Q_OBJECT
    
    // ===== 声明为单例 =====
    QML_ELEMENT
    QML_SINGLETON
    
    Q_PROPERTY(QString appName READ appName CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString buildTime READ buildTime CONSTANT)
    Q_PROPERTY(QStringList items READ items NOTIFY itemsChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemsChanged)
    
public:
    Q_DISABLE_COPY_MOVE(DataModel)
    
    // ===== 单例工厂方法 =====
    static DataModel* create(QQmlEngine *engine, QJSEngine *scriptEngine);
    
    QString appName() const { return app_name_; }
    QString version() const { return version_; }
    QString buildTime() const { return build_time_; }
    QStringList items() const { return items_; }
    int itemCount() const { return items_.count(); }
    
    Q_INVOKABLE void addItem(const QString &item);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void clearItems();
    Q_INVOKABLE QString getItemAt(int index) const;
    
signals:
    void itemsChanged();
    
private:
    // ===== 私有构造函数 =====
    explicit DataModel(QObject *parent = nullptr);
    
    QString app_name_;
    QString version_;
    QString build_time_;
    QStringList items_;
};

#endif // DATA_MODEL_H