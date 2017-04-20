import QtQuick 2.0

Rectangle {

    color: mrSaveResult.pressed ? Qt.darker(strColor, 1.2) : strColor

    radius: height * 0.1

    property bool isOrange: true

    property int labelSize: 15

    property string label
    property string strColor: isOrange ? "#FF5E2C" : "#838383"

    signal buttonClicked()

    MouseArea {
        id: mrSaveResult

        anchors.fill: parent

        onClicked: {
            buttonClicked()
        }
    }

    Text {
        id: labelButton

        text: label

        color: "white"

        font.pixelSize: labelSize

        anchors.centerIn: parent
    }
}
