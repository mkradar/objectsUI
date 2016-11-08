#ifndef RADAREXCEPTION_H
#define RADAREXCEPTION_H
#include <QException>
#include <QString>

class RadarException: public QException
{
private:
    QString _message;
public:
    RadarException(const RadarException& exception)
    {
        this->_message = exception.getMessage();
    }

    RadarException(QString message)
    {
        this->_message = message;
    }

    ~RadarException() throw()
    {
    }

    RadarException* clone() const
    {
        return new RadarException(*this);
    }

    void raise() const
    {
        throw *this;
    }

    QString getMessage() const
    {
        return this->_message;
    }
};

#endif // RADAREXCEPTION_H
