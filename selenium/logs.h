#ifndef LOGS_H
#define LOGS_H

#include <QObject>
#include <QJsonObject>

#include "logentry.h"

class WebDriverHub;
class Logs
{
public:
    Logs(WebDriverHub* hub);

    QStringList availableLogTypes();
    QList<LogEntry*> get(QString type);

private:
    WebDriverHub* m_hub;
};

#endif // LOGS_H
