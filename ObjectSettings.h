#ifndef OBJECTSETTINGS_H
#define OBJECTSETTINGS_H
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

class ObjectSettings
{
public:
    void LoadFromFile(const QString& fileName)
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
            return;
        }

        QTextStream in(&file);

        unsigned int lineCounter = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            switch(lineCounter)
            {
            case 0:
                this->rangeCellCount = line.toInt();
                break;
            case 1:
                this->signalsCount = line.toInt();
                break;
            case 2:
                this->distanceStep = line.toDouble();
                break;
            case 3:
                this->distanceOffset = line.toDouble();
                break;
            case 4:
                this->speedStep = line.toDouble();
                break;
            }

            lineCounter++;
        }
    }

    int rangeCellCount;
    int signalsCount;
    double distanceStep;
    double distanceOffset;
    double speedStep;
};

#endif // OBJECTSETTINGS_H
