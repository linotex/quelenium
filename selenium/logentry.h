#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QObject>

class LogEntry
{
public:
    LogEntry(QString level, int timestamp, QString message);

    int     timestamp() { return m_timestamp; }
    QString level()     { return m_level; }
    QString message()   { return m_message; }

private:
    QString m_level;
    int m_timestamp;
    QString m_message;
};

#endif // LOGENTRY_H
