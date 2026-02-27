// src/main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "qml_reloader.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // ===== 创建QML引擎 =====
    QQmlApplicationEngine engine;
    
    // ===== 定义QML源目录 =====
    QString qmlSourceDir = QML_SOURCE_DIR;
    qDebug() << "QML Source Directory:" << qmlSourceDir;
    
    // ===== 添加QML导入路径 =====
    engine.addImportPath(qmlSourceDir);
    
    // ===== 创建QML热重载器 =====
    QmlReloader reloader(&engine);
    
    // ===== 设置主QML文件 =====
    QString mainQmlFile = qmlSourceDir + "/main.qml";
    reloader.set_main_qml_file(QUrl::fromLocalFile(mainQmlFile));
    
    // ===== 监视QML文件变化 =====
    reloader.watch_directory(qmlSourceDir);
    
    // ===== 加载主QML文件 =====
    qDebug() << "Loading QML file:" << mainQmlFile;
    engine.load(QUrl::fromLocalFile(mainQmlFile));
    
    // ===== 检查是否成功加载 =====
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML file!";
        return -1;
    }
    
    qDebug() << "Application started successfully";
    qDebug() << "Watching for QML file changes...";
    
    // ===== 运行应用事件循环 =====
    return app.exec();
}