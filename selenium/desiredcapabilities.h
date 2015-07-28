#ifndef DESIREDCAPABILITIES_H
#define DESIREDCAPABILITIES_H

#include <QObject>
#include <QDebug>

#include "proxy.h"
#include "browsertype.h"

class DesiredCapabilities
{
public:
    DesiredCapabilities(BrowserType::BROWSER_TYPE browser);

    bool isTakesScreenshot()     { return m_takesScreenshot; }
    bool isHandlesAlerts()       { return m_handlesAlerts; }
    bool isCssSelectorsEnabled() { return m_cssSelectorsEnabled; }

    void setBrowser(BrowserType::BROWSER_TYPE browser)       { m_browserType = browser; }
    void setVersion(QString version)   { m_version = version; }
    void setPlatform(int platform)     { m_platform = platform; }

    void setDatabaseEnabled(bool enabled)          { m_databaseEnabled = enabled;           m_properties << "databaseEnabled"; }
    void setLocationContextEnabled(bool enabled)   { m_locationContextEnabled = enabled;    m_properties << "locationContextEnabled"; }
    void setApplicationCacheEnabled(bool enabled)  { m_applicationCacheEnabled = enabled;   m_properties << "applicationCacheEnabled"; }
    void setBrowserConnectionEnabled(bool enabled) { m_browserConnectionEnabled = enabled;  m_properties << "browserConnectionEnabled"; }
    void setWebStorageEnabled(bool enabled)        { m_webStorageEnabled = enabled;         m_properties << "webStorageEnabled"; }
    void setAcceptSslCerts(bool accept)            { m_acceptSslCerts = accept;             m_properties << "acceptSslCerts"; }
    void setNativeEvents(bool enabled)             { m_nativeEvents = enabled;              m_properties << "nativeEvents"; }
    void setUnexpectedAlertBehaviour(int value)    { m_unexpectedAlertBehaviour = value;    m_properties << "unexpectedAlertBehaviour"; }
    void setJavascriptEnabled(bool enabled)        { m_javascriptEnabled = enabled;         m_properties << "javascriptEnabled"; }      //only on HTMLUnitDriver
    void setRotatable(bool rotable)                { m_rotatable = rotable;                 m_properties << "rotatable"; }              //only applies to mobile platforms
    void setElementScrollBehavior(int value)       { m_elementScrollBehavior = value;       m_properties << "elementScrollBehavior"; }  //Supported in IE and Firefox (since 2.36)
    void setProxy(Proxy* proxy)              { m_proxy = proxy; }

    BrowserType::BROWSER_TYPE browser()  { return m_browserType; }
    Proxy* proxy() { return m_proxy; }

    bool isDatabaseEnabled()          { return getProperty("databaseEnabled"         , m_databaseEnabled).toBool(); }
    bool isLocationContextEnabled()   { return getProperty("locationContextEnabled"  , m_locationContextEnabled).toBool(); }
    bool isApplicationCacheEnabled()  { return getProperty("applicationCacheEnabled" , m_applicationCacheEnabled).toBool(); }
    bool isBrowserConnectionEnabled() { return getProperty("browserConnectionEnabled", m_browserConnectionEnabled).toBool(); }
    bool isWebStorageEnabled()        { return getProperty("webStorageEnabled"       , m_webStorageEnabled).toBool(); }
    bool isAcceptSslCerts()           { return getProperty("acceptSslCerts"          , m_acceptSslCerts).toBool(); }
    bool isNativeEvents()             { return getProperty("nativeEvents"            , m_nativeEvents).toBool(); }
    bool isJavascriptEnabled()        { return getProperty("javascriptEnabled"       , m_javascriptEnabled).toBool(); }
    bool isRotatable()                { return getProperty("rotatable"               , m_rotatable).toBool(); }
    int  unexpectedAlertBehaviour()   { return getProperty("unexpectedAlertBehaviour", m_unexpectedAlertBehaviour).toBool(); }
    int  elementScrollBehavior()      { return getProperty("elementScrollBehavior"   , m_elementScrollBehavior).toBool(); }

    bool hasDatabaseEnabled()          { return m_properties.contains("databaseEnabled"); }
    bool hasLocationContextEnabled()   { return m_properties.contains("locationContextEnabled"); }
    bool hasApplicationCacheEnabled()  { return m_properties.contains("applicationCacheEnabled"); }
    bool hasBrowserConnectionEnabled() { return m_properties.contains("browserConnectionEnabled"); }
    bool hasWebStorageEnabled()        { return m_properties.contains("webStorageEnabled"); }
    bool hasAcceptSslCerts()           { return m_properties.contains("acceptSslCerts"); }
    bool hasNativeEvents()             { return m_properties.contains("nativeEvents"); }
    bool hasJavascriptEnabled()        { return m_properties.contains("javascriptEnabled"); }
    bool hasRotatable()                { return m_properties.contains("rotatable"); }
    bool hasUnexpectedAlertBehaviour() { return m_properties.contains("unexpectedAlertBehaviour"); }
    bool hasElementScrollBehavior()    { return m_properties.contains("elementScrollBehavior"); }

protected:
    QVariant getProperty(QString property, QVariant value);
    bool     hasProperty(QString property);

    QStringList m_properties;

    bool m_takesScreenshot;
    bool m_handlesAlerts;
    bool m_cssSelectorsEnabled;

    BrowserType::BROWSER_TYPE m_browserType;
    int     m_platform;
    QString m_version;

    bool m_javascriptEnabled;//
    bool m_databaseEnabled;
    bool m_locationContextEnabled;
    bool m_applicationCacheEnabled;
    bool m_browserConnectionEnabled;
    bool m_webStorageEnabled;
    bool m_acceptSslCerts;
    bool m_rotatable;//
    bool m_nativeEvents;
    Proxy* m_proxy;
    int m_unexpectedAlertBehaviour;
    int m_elementScrollBehavior;//
};

#endif // DESIREDCAPABILITIES_H
