#include "options.h"
#include "webdriver.h"
#include "webdriverhub.h"

void Options::deleteAllCookies()
{
    m_hub->deleteAllCookie();
}

QList<Cookie*> Options::cookies()
{
    QList<Cookie*> cks;

    QList<QJsonObject> listCookies = m_hub->getAllCookie();
    for(int i = 0; i < listCookies.size(); i++)
    {
        QJsonObject c = listCookies.at(i);

        QString name   = c["name"].toString();
        QString value  = c["value"].toString();
        QString path   = c["path"].isString() ? c["path"].toString() : "";
        QString domain = c["domain"].isString() ? c["domain"].toString() : "";

        int time = c["expiry"].isDouble() ? c["expiry"].toInt() : 0;
        QDateTime expiry = QDateTime::fromTime_t(time);

        bool isSecure   = c["secure"].isBool() ? c["secure"].toBool() : false;
        bool isHttpOnly = c["httpOnly"].isBool() ? c["httpOnly"].toBool() : false;

        int hCode = c["hCode"].isDouble() ? c["hCode"].toInt() : 0;

        Cookie *cookie = new Cookie(name, value, path, domain, expiry, isSecure, isHttpOnly);
        cookie->setHashCode(hCode);

        cks.push_back(cookie);
    }

    return cks;
}

Cookie* Options::cookieNamed(QString name)
{
    QList<Cookie*> cks = cookies();
    Cookie* cookie = 0;
    for(int i = 0; i < cks.size(); i++)
    {
        if(cks.at(i)->name() == name) {
            cookie = cks.at(i);
            break;
        }
    }

    return cookie;
}

void Options::addCookie(Cookie *cookie)
{
    QJsonObject json;
    json["name"]     = QString(cookie->name());
    json["value"]    = QString(cookie->value());
    json["secure"]   = cookie->isSecure();
    json["httpOnly"] = cookie->isHttpOnly();
    json["expiry"]   = (int)(cookie->expiry().toTime_t());

    if(!cookie->path().isEmpty()) {
        json["path"] = cookie->path();
    }

    if(!cookie->domain().isEmpty()) {
        json["domain"] = cookie->domain();
    }

    m_hub->setCookie(json);
}

void Options::deleteCookie(Cookie *cookie)
{
    deleteCookieNamed(cookie->name());
}

void Options::deleteCookieNamed(QString name)
{
    m_hub->deleteCookie(name);
}

Timeouts* Options::timeouts()
{
    return new Timeouts(m_driver, m_hub);
}

Window* Options::window()
{
    return new Window(m_driver->windowHandle(), m_hub);
}

ImeHandler* Options::ime()
{
    return new ImeHandler(m_hub);
}

Logs* Options::logs()
{
    return new Logs(m_hub);
}
