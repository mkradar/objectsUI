#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFileDialog>
#include <QIcon>
#include <QtQml/QQmlContext>
#include "CalculationManager.h"
#include "GraphQuickItem.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<GraphQuickItem>("Graph", 1, 0, "Graph");

    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/resources/appIcon.ico"));

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/mainOBJ.qml")));
    QObject* applicationWindow = engine.rootObjects().first();

    CalculationManager manager;
    if(!manager.setApplicationWindow(applicationWindow))
    {
        return -1;
    }

    return app.exec();
}
