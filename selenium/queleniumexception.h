#ifndef QUELENIUMEXCEPTION_H
#define QUELENIUMEXCEPTION_H

#include <QObject>
#include <QException>

#include "responsestatuscodes.h"

class QueleniumException : public QException
{
public:
    explicit QueleniumException(QString message, int code = 0):
        QException(),
        m_message(message),
        m_code(code)
    {}
    ~QueleniumException() throw() {}

    QString message() { return m_message; }
    int     code()    { return m_code; }

    const char* what() const throw() { return m_message.toUtf8();}
    void raise() const { throw *this; }
    QueleniumException *clone() const { return new QueleniumException(*this); }

protected:
    QString m_message;
    int     m_code;
};

#endif // QUELENIUMEXCEPTION_H
