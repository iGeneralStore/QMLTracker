#include <QGuiApplication>
#include "qtquick2applicationviewer.h"
//#include <QQmlApplicationEngine>

#include <QtQuick/QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>

#include "filter.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    qmlRegisterType<Filter>("test.opencv.qt", 1, 0, "CVFilter");

    /*
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    */
    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QMLTracker/main.qml"));
    viewer.showFullScreen();

    return app.exec();
}
