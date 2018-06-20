import QtQuick 2.11
import QtQuick.Controls 2.4
import P2P 1.0

Page {
    id: page
    width: 600
    height: 400

    header: Label {
        text: qsTr("Page 1")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Label {
        text: qsTr("You are on Page 1.")
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: parent
            onClicked: parent.text = "Click"
        }
    }

    Text {
        anchors.centerIn: parent
        text: animatedValue
    }

    TextInput {
        id: textInput
        color: "#ffffff"
        text: qsTr("Text Input")
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 303
        anchors.top: parent.top
        anchors.topMargin: 6
        horizontalAlignment: Text.AlignRight
        font.family: "Times New Roman"
        font.pixelSize: 12
    }
}
