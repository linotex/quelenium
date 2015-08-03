#ifndef DESIREDCAPABILITIES_H
#define DESIREDCAPABILITIES_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

#include "proxy.h"
#include "browsertype.h"

class DesiredCapabilities
{
public:
    DesiredCapabilities(BrowserType::BROWSER_TYPE browser);

    void setBrowser(BrowserType::BROWSER_TYPE browser)  { m_browserType = browser; }
    void setProxy(Proxy* proxy)                         { m_proxy = proxy; }
    void setVersion(QString version)                    { m_version = version; }
    void setPlatform(int platform)                      { m_platform = platform;}

    BrowserType::BROWSER_TYPE browser()  { return m_browserType; }
    Proxy*  proxy()                       { return m_proxy; }
    int     platform()                    { return m_platform; }
    QString version()                     { return m_version; }

    void setDatabaseEnabled(bool enabled)          { setProperty("databaseEnabled",             enabled); }
    void setLocationContextEnabled(bool enabled)   { setProperty("locationContextEnabled",      enabled); }
    void setApplicationCacheEnabled(bool enabled)  { setProperty("applicationCacheEnabled",     enabled); }
    void setBrowserConnectionEnabled(bool enabled) { setProperty("browserConnectionEnabled",    enabled); }
    void setWebStorageEnabled(bool enabled)        { setProperty("webStorageEnabled",           enabled); }
    void setAcceptSslCerts(bool accept)            { setProperty("acceptSslCerts",              accept);  }
    void setNativeEvents(bool enabled)             { setProperty("nativeEvents",                enabled); }
    void setUnexpectedAlertBehaviour(int value)    { setProperty("unexpectedAlertBehaviour",    value);   }
    void setJavascriptEnabled(bool enabled)        { setProperty("javascriptEnabled",           enabled); }  //only on HTMLUnitDriver
    void setRotatable(bool rotable)                { setProperty("rotatable",                   rotable); }  //only applies to mobile platforms
    void setElementScrollBehavior(int value)       { setProperty("elementScrollBehavior",       value);   }  //Supported in IE and Firefox (since 2.36)
    void setCssSelectorsEnabled(bool enabled)      { setProperty("cssSelectorsEnabled",         enabled); }
    void setHandlesAlerts(bool handle)             { setProperty("handlesAlerts",               handle);  }
    void setTakesScreenshot(bool take)             { setProperty("takesScreenshot",             take);    }

    bool isTakesScreenshot()          { return property("takesScreenshot").toBool(); }    //Read-only from server
    bool isHandlesAlerts()            { return property("handlesAlerts").toBool(); }      //Read-only from server
    bool isCssSelectorsEnabled()      { return property("cssSelectorsEnabled").toBool(); }//Read-only from server
    bool isDatabaseEnabled()          { return property("databaseEnabled").toBool(); }
    bool isLocationContextEnabled()   { return property("locationContextEnabled").toBool(); }
    bool isApplicationCacheEnabled()  { return property("applicationCacheEnabled").toBool(); }
    bool isBrowserConnectionEnabled() { return property("browserConnectionEnabled").toBool(); }
    bool isWebStorageEnabled()        { return property("webStorageEnabled").toBool(); }
    bool isAcceptSslCerts()           { return property("acceptSslCerts").toBool(); }
    bool isNativeEvents()             { return property("nativeEvents").toBool(); }
    bool isJavascriptEnabled()        { return property("javascriptEnabled").toBool(); }
    bool isRotatable()                { return property("rotatable").toBool(); }
    int  unexpectedAlertBehaviour()   { return property("unexpectedAlertBehaviour").toInt(); }
    int  elementScrollBehavior()      { return property("elementScrollBehavior").toInt(); }

    QJsonObject properties() { return m_properties; }

protected:
    QVariant property(QString property);
    void     setProperty(QString property, QString value);
    void     setProperty(QString property, int value);
    void     setProperty(QString property, bool value);

    bool     hasProperty(QString property);

    //QStringList m_properties;
    QJsonObject m_properties;

    BrowserType::BROWSER_TYPE m_browserType;
    Proxy*  m_proxy;
    QString m_version;
    int     m_platform;
};

#endif // DESIREDCAPABILITIES_H
