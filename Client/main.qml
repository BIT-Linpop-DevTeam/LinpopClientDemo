import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0
import EmojiModel 1.0

ApplicationWindow {
    width: 400
    height: 420
    visible: true
    title: 'Emoji Picker'
    signal qmlSignal(string message)
//    qmlSignal:"Hello from QML!"
    EmojiModel {
        id: emojiModel
        iconsPath: 'emojiSvgs/'
        iconsType: '.svg'
    }
    Rectangle {
        id: body
        width: 400
        height: 420
        radius: 10
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        EmojiPicker {
            id: emojiPicker
            model: emojiModel
            anchors.fill: parent
        }
    }
    DropShadow {
        anchors.fill: body
        horizontalOffset: 12
        verticalOffset: 12
        radius: 16
        samples: radius * 2 + 1
        color: Qt.rgba(0, 0, 0, 0.1)
        source: body
    }
    DropShadow {
        anchors.fill: body
        horizontalOffset: -8
        verticalOffset: -8
        radius: 16
        samples: radius * 2 + 1
        color: Qt.rgba(0, 0, 0, 0.1)
        source: body
    }
}
