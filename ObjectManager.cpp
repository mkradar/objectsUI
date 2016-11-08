#include "ObjectManager.h"
#include <QVariant>
#include <QUrl>
#include <QMessageBox>
#include <QQuickItem>
#include "RadarException.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
    if(this->_settings)
    {
        delete this->_settings;
    }

    if(this->_signal)
    {
        delete this->_signal;
    }
}

bool ObjectManager::setApplicationWindow(QObject* applicationWindow)
{
    if(!applicationWindow)
    {
        QMessageBox::information(0, "error", "Application window couldn't be null");
        return false;
    }
    this->_applicationWindow = applicationWindow;
    QObject* dialog = applicationWindow->findChild<QObject*>("settingsFileDialog");
    if(!dialog)
    {
        QMessageBox::information(0, "error", "Settings file dialog couldn't be null");
        return false;
    }
    QObject::connect(dialog,SIGNAL(accepted()),this,SLOT(settingsFilenameSlot()));
    this->_settingsFileDialog = dialog;

    dialog = applicationWindow->findChild<QObject*>("signalsFileDialog");
    if(!dialog)
    {
        QMessageBox::information(0, "error", "Signals file dialog couldn't be null");
        return false;
    }
    QObject::connect(dialog,SIGNAL(accepted()),this,SLOT(signalFilenameSlot()));
    this->_signalFileDialog = dialog;

    dialog = applicationWindow->findChild<QObject*>("analyzeBtn");
    if(!dialog)
    {
        QMessageBox::information(0, "error", "Analyze button couldn't be null");
        return false;
    }
    QObject::connect(dialog,SIGNAL(clicked()),this,SLOT(analyzeBtnClickedSlot()));

    /* this->_graph = applicationWindow->findChild<QQuickItem*>("graph");
    if(!this->_graph)
    {
        QMessageBox::information(0, "error", "graph couldn't be null");
        return false;
    }*/
    return true;
}

void ObjectManager::settingsFilenameSlot()
{
    if(this->_settings)
    {
        delete this->_settings;
    }
    this->_settings = new ObjectSettings();
    this->_settings->LoadFromFile(this->_settingsFileDialog->property("fileUrl").toUrl().toLocalFile());
    this->renderSettingsValues();
    this->renderSignalValues();
}

void ObjectManager::signalFilenameSlot()
{
    if(this->_signal)
    {
        delete this->_signal;
    }
    this->_signal = new Signals(this->_signalFileDialog->property("fileUrl").toUrl().toLocalFile());
    this->renderSignalValues();
}

void ObjectManager::analyzeBtnClickedSlot()
{
    try
    {
    }
    catch(const RadarException& exception)
    {
        QMessageBox::information(0, "error", exception.getMessage());
    }
}

void ObjectManager::renderSettingsValues()
{
    if(!this->_settings)
    {
        QMessageBox::information(0, "error", "Please initialize settings first");
        return;
    }
    QQuickItem* rangeCellCountInput = this->_applicationWindow->findChild<QQuickItem*>("rangeCellCountInput");
    rangeCellCountInput->setProperty("text", this->_settings->rangeCellCount);
    QQuickItem* signalsCountInput = this->_applicationWindow->findChild<QQuickItem*>("signalsCountInput");
    signalsCountInput->setProperty("text", this->_settings->signalsCount);
    QQuickItem* distanceStepInput = this->_applicationWindow->findChild<QQuickItem*>("distanceStepInput");
    distanceStepInput->setProperty("text", this->_settings->distanceStep);
}

void ObjectManager::renderSignalValues()
{
    QQuickItem* signalsStatusRect = this->_applicationWindow->findChild<QQuickItem*>("signalsStatusRect");
    QQuickItem* signalsStatusText = this->_applicationWindow->findChild<QQuickItem*>("signalsStatusText");

    if(!this->_signal)
    {
        signalsStatusRect->setProperty("color", "#FFFF00");
        signalsStatusText->setProperty("text", "Please load the signals");
        return;
    }
    QQuickItem* signalsCountValue = this->_applicationWindow->findChild<QQuickItem*>("signalsCountValue");
    signalsCountValue->setProperty("text", this->_signal->getSignalsCount());

    if(!this->_settings)
    {
        signalsStatusRect->setProperty("color", "#FFFF00");
        signalsStatusText->setProperty("text", "Please load the settings");
        return;
    }

    int expectedSignalsCount = this->_settings->rangeCellCount * this->_settings->signalsCount;

    if(this->_signal->getSignalsCount() != expectedSignalsCount)
    {
        signalsStatusRect->setProperty("color", "#CC0000");
        signalsStatusText->setProperty("text", "Expected signals count: " + QString::number(expectedSignalsCount));
        return;
    }

    signalsStatusRect->setProperty("color", "#66CC33");
    signalsStatusText->setProperty("text", "Signals are correct");
}
