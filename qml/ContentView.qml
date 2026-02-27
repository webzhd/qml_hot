// qml/ContentView.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.example.models 1.0

Item {
    id: content
    
    ColumnLayout {
        id: mainLayout
        
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            
            color: "#F2F2F7"
            radius: 8
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8
                
                Text {
                    text: "📱 应用信息"
                    color: "#000000"
                    font.bold: true
                    font.pixelSize: 14
                }
                
                RowLayout {
                    spacing: 16
                    
                    Text {
                        text: "名称: " + DataModel.appName
                        color: "#666666"
                        font.pixelSize: 12
                    }
                    
                    Text {
                        text: "版本: " + DataModel.version
                        color: "#666666"
                        font.pixelSize: 12
                    }
                    
                    Text {
                        text: "构建: " + DataModel.buildTime
                        color: "#666666"
                        font.pixelSize: 12
                    }
                }
            }
        }
        
        Text {
            id: titleText
            
            Layout.alignment: Qt.AlignHCenter
            
            text: "计数器演 示111"
            color: "#000000"
            font.pixelSize: 28
            font.bold: true
        }
        
        Column {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8
            
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "CURRENT COUNT"
                color: "#666666"
                font.pixelSize: 12
            }
            
            Text {
                id: counterDisplay
                anchors.horizontalCenter: parent.horizontalCenter
                text: String(Application.count)
                color: "#007AFF"
                font.pixelSize: 64
                font.bold: true
            }
        }
        
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 16
            
            Rectangle {
                width: 100
                height: 44
                color: mouseArea1.containsMouse ? "#0051D5" : "#007AFF"
                radius: 8
                
                MouseArea {
                    id: mouseArea1
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: Application.decrement()
                }
                
                Text {
                    anchors.centerIn: parent
                    text: "- Decrease"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
            
            Rectangle {
                width: 100
                height: 44
                color: mouseArea2.containsMouse ? "#0051D5" : "#007AFF"
                radius: 8
                
                MouseArea {
                    id: mouseArea2
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: Application.increment()
                }
                
                Text {
                    anchors.centerIn: parent
                    text: "+ Increase"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
            
            Rectangle {
                width: 80
                height: 44
                color: mouseArea3.containsMouse ? "#FF5050" : "#FF3B30"
                radius: 8
                
                MouseArea {
                    id: mouseArea3
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: Application.reset()
                }
                
                Text {
                    anchors.centerIn: parent
                    text: "Reset"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
        }
        
        Text {
            text: "📋 数据列表 (共 " + DataModel.itemCount + " 项)"
            color: "#000000"
            font.bold: true
            font.pixelSize: 14
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            color: "#F9F9F9"
            border.color: "#E5E5EA"
            border.width: 1
            radius: 8
            
            ListView {
                anchors.fill: parent
                anchors.margins: 8
                
                model: DataModel.items
                spacing: 8
                clip: true
                
                delegate: Rectangle {
                    width: parent.width - 16
                    height: 44
                    
                    color: mouseAreaItem.containsMouse ? "#E8E8FF" : "#FFFFFF"
                    radius: 6
                    border.color: "#E5E5EA"
                    border.width: 1
                    
                    MouseArea {
                        id: mouseAreaItem
                        anchors.fill: parent
                        hoverEnabled: true
                        
                        onClicked: {
                            DataModel.removeItem(index)
                        }
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8
                        
                        Text {
                            text: (index + 1) + "."
                            color: "#999999"
                            font.pixelSize: 12
                        }
                        
                        Text {
                            Layout.fillWidth: true
                            text: modelData
                            color: "#000000"
                            font.pixelSize: 13
                        }
                        
                        Text {
                            text: "✕"
                            color: "#FF3B30"
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }
                }
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            
            TextField {
                id: inputField
                Layout.fillWidth: true
                placeholderText: "输入新项目..."
                
                background: Rectangle {
                    color: "#FFFFFF"
                    border.color: "#E5E5EA"
                    border.width: 1
                    radius: 6
                }
                
                Keys.onReturnPressed: {
                    DataModel.addItem(text)
                    text = ""
                }
            }
            
            Rectangle {
                width: 100
                height: 40
                
                color: addButtonMouse.containsMouse ? "#0051D5" : "#007AFF"
                radius: 6
                
                MouseArea {
                    id: addButtonMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    
                    onClicked: {
                        DataModel.addItem(inputField.text)
                        inputField.text = ""
                        inputField.focus = true
                    }
                }
                
                Text {
                    anchors.centerIn: parent
                    text: "添 加"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
            
            Rectangle {
                width: 80
                height: 40
                
                color: clearButtonMouse.containsMouse ? "#FF5050" : "#FF3B30"
                radius: 6
                
                MouseArea {
                    id: clearButtonMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    
                    onClicked: {
                        DataModel.clearItems()
                    }
                }
                
                Text {
                    anchors.centerIn: parent
                    text: "清空"
                    color: "#FFFFFF"
                    font.pixelSize: 12
                    font.bold: true
                }
            }
        }
    }
}