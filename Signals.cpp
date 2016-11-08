#include "Signals.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

Signals::Signals(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        this->_signalEntries.append(in.readLine().toDouble());
    }
}

int Signals::getSignalsCount()
{
    return this->_signalEntries.size();
}

double Signals::getAt(int index)
{
    return this->_signalEntries[index];
}
