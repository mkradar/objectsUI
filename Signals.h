#ifndef SIGNAL_H
#define SIGNAL_H
#include <QString>
#include <QList>

class Signals
{
public:
    Signals(const QString& fileName);

    int getSignalsCount();

    double getAt(int index);

private:
    QList<double> _signalEntries;
};

#endif // SIGNAL_H
