// src/main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "qml_reloader.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // ===== 设置Qt Quick Controls样式 =====
    qputenv("QT_QUICK_CONTROLS_STYLE", "Material");
    
    // ===== 创建QML引擎 =====
    QQmlApplicationEngine engine;
    
    QString qmlSourceDir = QML_SOURCE_DIR;
    qDebug() << "QML Source Directory:" << qmlSourceDir;
    
    engine.addImportPath(qmlSourceDir);
    
    // ===== 创建热重载器 =====
    QmlReloader reloader(&engine);
    QString mainQmlFile = qmlSourceDir + "/main.qml";
    reloader.set_main_qml_file(QUrl::fromLocalFile(mainQmlFile));
    reloader.watch_directory(qmlSourceDir);
    
    // ===== 加载主QML文件 =====
    qDebug() << "Loading QML file:" << mainQmlFile;
    engine.load(QUrl::fromLocalFile(mainQmlFile));
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML file!";
        return -1;
    }
    
    qDebug() << "Application started successfully";
    
    // ===== 关键：在启动后立即加载初始的ContentView.qml =====
    // 延迟0ms确保QML已完全初始化
    QTimer::singleShot(0, &reloader, [&reloader, qmlSourceDir]() {
        qDebug() << "Loading initial ContentView.qml...";
        
        // 直接调用reload_with_component来加载初始内容
        reloader.loadInitialContent(qmlSourceDir + "/ContentView.qml");
    });
    
    return app.exec();
}