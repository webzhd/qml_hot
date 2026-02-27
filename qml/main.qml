// qml/main.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import com.example.models 1.0

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    
    visible: true
    width: 700
    height: 600
    minimumWidth: 500
    minimumHeight: 400
    
    title: "Qt Quick Hot Reload Demo"
    color: "#FFFFFF"
    
    Material.primary: "#007AFF"
    
    // ===== 空的Loader，将由C++填充 =====
    Loader {
        id: contentLoader
        objectName: "contentLoader"
        anchors.fill: parent
    }
}