import QtQuick 2.0
import QtDataVisualization 1.2



Rectangle {
    id:mainview
    width: 640
    height: 480
    color: surfacePlot.theme.windowColor


    Item {
        id: surfaceView
        width: mainview.width
        height: mainview.height
        anchors.top: mainview.top
        anchors.left: mainview.left

        ColorGradient {
            id: surfaceGradient
            ColorGradientStop { position: 0.0; color: "darkslategray" }
            ColorGradientStop { id: middleGradient; position: 0.25; color: "peru" }
            ColorGradientStop { position: 1.0; color: "red" }
        }

        Surface3D {
            id: surfacePlot
            width: surfaceView.width
            height: surfaceView.height

            theme: Theme3D {
                type: Theme3D.ThemeStoneMoss
                font.family: "STCaiyun"
                font.pointSize: 35
                colorStyle: Theme3D.ColorStyleRangeGradient
                baseGradients: [surfaceGradient]
            }

            shadowQuality: AbstractGraph3D.ShadowQualityMedium
//            selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndRow
            scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeft
            axisY.min: 0.0
            axisY.max: 10.0
//            axisX.segmentCount: 10
//            axisX.subSegmentCount: 2
//            axisX.labelFormat: "%i"
//            axisZ.segmentCount: 10
//            axisZ.subSegmentCount: 2
//            axisZ.labelFormat: "%i"
//            axisY.segmentCount: 5
//            axisY.subSegmentCount: 2
//            axisY.labelFormat: "%i"
//            axisY.title: "Height"
//            axisX.title: "Latitude"
//            axisZ.title: "Longitude"


            Surface3DSeries {
                itemLabelFormat: "Pop density at (@xLabel N, @zLabel E): @yLabel"
                ItemModelSurfaceDataProxy {
                    itemModel: cityModel
                    // Mapping model roles to surface series rows, columns, and values.
                    rowRole: "xdata"
                    columnRole: "ydata"
                    yPosRole: "zdata"
                }
            }
        }
    }


//        ListView {
//              width: parent.width;
//              height: parent.height;
//              anchors.centerIn: parent;

//            model: cityModel;
//             delegate: Text {
//                  text:model.ydata;
//                 font.pixelSize: 50;
//                 color: "red";
//             }
//        }





//    ListModel {
//        id: dataModel
//        ListElement{ longitude: "20"; latitude: "10"; pop_density: "4.75"; }
//        ListElement{ longitude: "21"; latitude: "10"; pop_density: "3.00"; }
//        ListElement{ longitude: "22"; latitude: "10"; pop_density: "1.24"; }
//        ListElement{ longitude: "23"; latitude: "10"; pop_density: "2.53"; }
//        ListElement{ longitude: "20"; latitude: "11"; pop_density: "2.55"; }
//        ListElement{ longitude: "21"; latitude: "11"; pop_density: "2.03"; }
//        ListElement{ longitude: "22"; latitude: "11"; pop_density: "3.46"; }
//        ListElement{ longitude: "23"; latitude: "11"; pop_density: "5.12"; }
//        ListElement{ longitude: "20"; latitude: "12"; pop_density: "1.37"; }
//        ListElement{ longitude: "21"; latitude: "12"; pop_density: "2.98"; }
//        ListElement{ longitude: "22"; latitude: "12"; pop_density: "3.33"; }
//        ListElement{ longitude: "23"; latitude: "12"; pop_density: "3.23"; }
//        ListElement{ longitude: "20"; latitude: "13"; pop_density: "4.34"; }
//        ListElement{ longitude: "21"; latitude: "13"; pop_density: "3.54"; }
//        ListElement{ longitude: "22"; latitude: "13"; pop_density: "1.65"; }
//        ListElement{ longitude: "23"; latitude: "13"; pop_density: "2.67"; }
//    }
}



