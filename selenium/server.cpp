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

QList<Session*> Server::sessions()
{
    QJsonArray jsonSessions = m_hub->sessions();

    QList<Session*> list;

    for(int i = 0; i < jsonSessions.size(); i++) {
        QJsonObject jsonSession = jsonSessions.at(i).toObject();
        QJsonObject jsonCap = jsonSession["capabilities"].toObject();

        DesiredCapabilities* c = new DesiredCapabilities(Browser::fromString(jsonCap["browserName"].toString()));
        c->setPlatform(Platform::fromString(jsonCap["platform"].toString()));
        c->setVersion(jsonCap["version"].toString());

        if(jsonCap["acceptSslCerts"].isBool()) {
            c->setAcceptSslCerts(jsonCap["acceptSslCerts"].toBool());
        }

        if(jsonCap["applicationCacheEnabled"].isBool()) {
            c->setApplicationCacheEnabled(jsonCap["applicationCacheEnabled"].toBool());
        }

        if(jsonCap["cssSelectorsEnabled"].isBool()) {
            c->setCssSelectorsEnabled(jsonCap["cssSelectorsEnabled"].toBool());
        }

        if(jsonCap["databaseEnabled"].isBool()) {
            c->setDatabaseEnabled(jsonCap["databaseEnabled"].toBool());
        }

        if(jsonCap["handlesAlerts"].isBool()) {
            c->setHandlesAlerts(jsonCap["handlesAlerts"].toBool());
        }

        if(jsonCap["javascriptEnabled"].isBool()) {
            c->setJavascriptEnabled(jsonCap["javascriptEnabled"].toBool());
        }

        if(jsonCap["locationContextEnabled"].isBool()) {
            c->setLocationContextEnabled(jsonCap["locationContextEnabled"].toBool());
        }

        if(jsonCap["nativeEvents"].isBool()) {
            c->setNativeEvents(jsonCap["nativeEvents"].toBool());
        }

        if(jsonCap["rotatable"].isBool()) {
            c->setRotatable(jsonCap["rotatable"].toBool());
        }

        if(jsonCap["takesScreenshot"].isBool()) {
            c->setTakesScreenshot(jsonCap["takesScreenshot"].toBool());
        }

        if(jsonCap["webStorageEnabled"].isBool()) {
            c->setWebStorageEnabled(jsonCap["webStorageEnabled"].toBool());
        }

        list.push_back(new Session(jsonSession["id"].toString(), c));
    }

    return list;
}
