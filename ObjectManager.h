#ifndef OBJECTMANAGER_H
#define CALCULATIONMANAGER_H
#include <QObject>
#include "ObjectSettings.h"
#include "Signals.h"

class ObjectManager : public QObject
{
    Q_OBJECT
public:
    ObjectManager();
    virtual ~ObjectManager();

    bool setApplicationWindow(QObject* applicationWindow);

signals:
    void valueCalculated(double newValue);

private:
    QObject* _applicationWindow;
    QObject* _settingsFileDialog;
    QObject* _signalFileDialog;

    ObjectSettings* _settings = NULL;
    Signals* _signal = NULL;

    void renderSettingsValues();
    void renderSignalValues();

public slots:
    void settingsFilenameSlot();
    void signalFilenameSlot();
    void analyzeBtnClickedSlot();
};

#endif // CALCULATIONMANAGER_H
