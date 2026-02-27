// src/main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "qml_hot_reload_system.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "Material");
    QGuiApplication app(argc, argv);

    // ===== 创建QML引擎 =====
    QQmlApplicationEngine engine;

    // ===== 定义QML源目录 =====
    QString qmlSourceDir = QML_SOURCE_DIR;
    qDebug() << "QML Source Directory:" << qmlSourceDir;

    // ===== 添加QML导入路径 =====
    engine.addImportPath(qmlSourceDir);

    // ===== 加载主QML文件 =====
    engine.load(QUrl::fromLocalFile(qmlSourceDir + "/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML file!";
        return -1;
    }

    // ===== 启动热更新系统 =====
    QmlHotReloadSystem reloadSystem(&engine);
    reloadSystem.watchDirectory(qmlSourceDir);
    reloadSystem.addLoader("contentLoader", qmlSourceDir + "/ContentView.qml");

    qDebug() << "Application started successfully";
    qDebug() << "Watching for QML file changes...";

    // ===== 运行应用事件循环 =====
    return app.exec();
}