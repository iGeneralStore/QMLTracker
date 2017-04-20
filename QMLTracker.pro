QT       += core gui quick qml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMLTracker
TEMPLATE = app

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    filter.cpp \
    opencvhelper.cpp

CONFIG += mobility
MOBILITY =

osx {

LIBS += -L/usr/local/lib -L/usr/local/Cellar/opencv3/3.1.0_4/share/OpenCV/3rdparty/lib

QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv

}

android {
INCLUDEPATH += "$$_PRO_FILE_PWD_/OpenCV-android-sdk/sdk/native/jni/include"
    LIBS += \
        -L"$$_PRO_FILE_PWD_/OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a"\
        -L"$$_PRO_FILE_PWD_/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a"\
        -llibtiff\
        -llibjpeg\
        -llibjasper\
        -llibpng\
        -lIlmImf\
        -ltbb\
        -lopencv_core\
        -lopencv_androidcamera\
        -lopencv_flann\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_features2d\
        -lopencv_calib3d\
        -lopencv_ml\
        -lopencv_objdetect\
        -lopencv_video\
        -lopencv_contrib\
        -lopencv_photo\
        -lopencv_java\
        -lopencv_legacy\
        -lopencv_ocl\
        -lopencv_stitching\
        -lopencv_superres\
        -lopencv_ts\
        -lopencv_videostab\

    ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android
}



# Add more folders to ship with the application, here
folder_01.source = qml/QMLTracker
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    filter.h \
    rgbframehelper.h \
    opencvhelper.h

DISTFILES += \
    qml/QMLTracker/Rect.qml
