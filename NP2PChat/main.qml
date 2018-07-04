import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
            Button {
                id: button
                text: qsTr("Button")
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.bottomMargin: 262
                anchors.rightMargin: 69

                Image {
                    id: img
                    //anchors.fill: parent
                    source: "/Original_bug.png"
                    smooth: true
                    visible: false
                }

                Image {
                    id: mask
                    source: "/Origin.png"
                    smooth: true
                    visible: false
                }

                Image{
                    id: rotateMask
                    source: mask
                    visible: true
                }

                OpacityMask {
                    id: om
                    anchors.fill: parent
                    source: img
                    maskSource: mask

                }

                RotationAnimation {
                    id: rotationAnimation
                    target: om
                    to: 90
                    direction: RotationAnimation.Clockwise
                    duration: 3000
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked:rotationAnimation.start()

                }
            }
        }

        Page2Form {

        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }
}
