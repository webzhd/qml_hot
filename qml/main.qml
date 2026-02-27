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

    Loader {
        id: contentLoader
        objectName: "contentLoader"
        anchors.fill: parent

        onStatusChanged: {
            if (status === Loader.Error) {
                console.error("❌ [LOADER] Error:", errorString)
            } else if (status === Loader.Ready) {
                console.log("✓ [LOADER] Ready")
            } else if (status === Loader.Loading) {
                console.log("[LOADER] Loading...")
            }
        }

        onSourceComponentChanged: {
            console.log("✓✓ [LOADER] sourceComponent changed, status:", status)
        }

        onLoaded: {
            console.log("✓✓✓ [LOADER] Item loaded and rendered!")
        }
    }
}