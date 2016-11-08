TEMPLATE = app

QT += qml quick widgets
QT += datavisualization

SOURCES += main.cpp \
    CalculationManager.cpp \
    Signals.cpp \
    ObjectManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    CalculationManager.h \
    Settings.h \
    Signals.h \
    BaseAnalysis.h \
    FftAlgorithm.h \
    RadarException.h \
    GraphQuickItem.h \
    GraphNode.h \
    GridNode.h \
    LineNode.h \
    LineShader.h \
    BezierSpline.h \
    LineVertex.h \
    ObjectManager.h \
    ObjectSettings.h

OTHER_FILES +=
