import QtQuick 2.0
import QtMultimedia 5.5
import QtQuick 2.1
import test.opencv.qt 1.0

Rectangle {
    id: root
    width: 600
    height: 680

    property var filterID: filter

    VideoOutput {
        id: output
        width: parent.width
        height: parent.height

        source: camera

        filters: filterID

        anchors.top: parent.top

        MouseArea {
            id: mr
            anchors.fill: parent

            onClicked: {

                var tX, tY

                tX = mouse.x
                tY = mouse.y

                var widthRate = output.sourceRect.width / output.width
                var heightRate = output.sourceRect.height / output.height

                var contentHeight = output.sourceRect.height
                filter.addPoint(tX*widthRate, contentHeight-tY*heightRate)
/*
                var com = Qt.createComponent("Rect.qml")
                var ret = com.createObject(root, {"x": tX, "y": tY})
*/
                filter.addPoint(tX, tY)
            }
        }



        CVFilter {
            id: filter
            active: true
        }

        Item {
            enabled: false
            Camera {
                id: camera
                imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
                captureMode: Camera.CaptureStillImage
                position: Camera.BackFace

                objectName: "qrCameraQML"

                onCameraStatusChanged: {
                    console.log("camera status : " + cameraStatus)
                }

                exposure {
                    exposureCompensation: -1.0
                    exposureMode: Camera.ExposurePortrait
                }

                Component.onCompleted: {
                    camera.start()
                }
            }
        }

    }
}

