#include "logentry.h"

LogEntry::LogEntry(QString level, int timestamp, QString message) :
    m_level(level),
    m_timestamp(timestamp),
    m_message(message)
{
}
