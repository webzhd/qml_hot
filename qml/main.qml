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

    // ===== 热更新函数：安全地替换 Component =====
    function setContentComponent(component) {
        console.log("🔄 QML: setContentComponent called");
        
        if (!component) {
            console.error("❌ component is null!");
            return;
        }

        // ===== 步骤1：清除旧 component 实例（但不销毁 C++ 对象）=====
        if (contentLoader.sourceComponent !== null) {
            console.log("🧹 Clearing old component reference");
            // 只是清除引用，不销毁对象（因为是 C++ 创建的）
            contentLoader.sourceComponent = null;
        }

        // ===== 步骤2：设置新 component =====
        console.log("✓ Setting new component");
        contentLoader.sourceComponent = component;
    }

    Loader {
        id: contentLoader
        objectName: "contentLoader"
        anchors.fill: parent

        onStatusChanged: {
            if (status === Loader.Error) {
                console.error("❌ contentLoader: Error:", errorString)
            } else if (status === Loader.Ready) {
                console.log("✓✓ contentLoader: Component loaded and ready")
            } else if (status === Loader.Loading) {
                console.log("⏳ contentLoader: Loading...")
            }
        }
        
        onLoaded: {
            console.log("✓✓✓ contentLoader: Item instantiated and rendered")
        }
    }
}