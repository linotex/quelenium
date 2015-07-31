#include "server.h"
#include "webdriverhub.h"

Server::Server(WebDriverHub* hub)
{
    m_hub = new WebDriverHub(hub->host(), hub->port(), hub->capabilities(false));
}

Status* Server::status()
{
    QJsonObject jsonStatus = m_hub->status();

    Os* os       = new Os(jsonStatus["os"].toObject());
    Build* build = new Build(jsonStatus["build"].toObject());

    Java* java = 0;
    if(jsonStatus["java"].isObject()) {
        java = new Java(jsonStatus["java"].toObject());
    }

    return new Status(os, build, java);
}
