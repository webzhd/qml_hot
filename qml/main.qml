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
        source: "ContentView.qml"

        onStatusChanged: {
            if (status === Loader.Error) {
                console.error("contentLoader: Error loading source:", source)
            } else if (status === Loader.Ready) {
                console.log("contentLoader: Successfully loaded:", source)
            }
        }
    }
}