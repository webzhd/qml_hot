// qml/main.qml
import QtQuick
import QtQuick.Controls
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

    // ===== 属性：存储ContentView.qml的完整路径 =====
    property string contentViewPath: ""

    // ===== 热更新函数（从C++调用）=====
    function reloadContent() {
        console.log("🔄 QML: reloadContent called with path:", mainWindow.contentViewPath);
        
        if (mainWindow.contentViewPath === "") {
            console.error("❌ contentViewPath not set!");
            return;
        }
        
        // 步骤1：先设置为空以重置状态
        contentLoader.source = "";
        
        // 步骤2：强制事件循环处理
        contentLoader.forceActiveFocus();
        
        // 步骤3：设置新的源（使用完整路径）
        contentLoader.source = mainWindow.contentViewPath;
    }

    Loader {
        id: contentLoader
        objectName: "contentLoader"
        anchors.fill: parent

        onStatusChanged: {
            if (status === Loader.Error) {
                console.error("❌ contentLoader: Error loading source:", source);
                console.error("   Error:", errorString);
            } else if (status === Loader.Ready) {
                console.log("✓ contentLoader: Successfully loaded:", source);
            } else if (status === Loader.Loading) {
                console.log("⏳ contentLoader: Loading:", source);
            }
        }
        
        onLoaded: {
            console.log("✓✓ contentLoader: Item fully loaded and rendered");
        }
    }
}