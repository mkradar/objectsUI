#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H
#include <QObject>
#include "Settings.h"
#include "Signals.h"

class CalculationManager : public QObject
{
    Q_OBJECT
public:
    CalculationManager();
    virtual ~CalculationManager();

    bool setApplicationWindow(QObject* applicationWindow);

signals:
    void valueCalculated(double newValue);

private:
    QObject* _applicationWindow;
    QObject* _settingsFileDialog;
    QObject* _signalFileDialog;
    QObject* _graph;

    Settings<4096, 64>* _settings = NULL;
    Signals* _signal = NULL;

    void renderSettingsValues();
    void renderSignalValues();

public slots:
    void settingsFilenameSlot();
    void signalFilenameSlot();
    void analyzeBtnClickedSlot();
};

#endif // CALCULATIONMANAGER_H
