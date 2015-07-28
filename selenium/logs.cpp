#include "logs.h"
#include "webdriverhub.h"

Logs::Logs(WebDriverHub *hub):
    m_hub(hub)
{
}

QStringList Logs::availableLogTypes()
{
    return m_hub->getLogTypes();
}

QList<LogEntry*> Logs::get(QString type)
{
    QList<LogEntry*> logs;

    QList<QJsonObject> jsons = m_hub->getLog(type);
    for(int i = 0; i < jsons.size(); i++)
    {
        LogEntry* log = new LogEntry(jsons.at(i)["level"].toString(),
                                     jsons.at(i)["timestamp"].toInt(),
                                     jsons.at(i)["message"].toString());
        logs.push_back(log);
    }

    return logs;
}
