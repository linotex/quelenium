#ifndef PROXY_H
#define PROXY_H

#include <QObject>
#include <QException>
#include <QDebug>

class Proxy
{
public:

    enum PROXY_TYPE {
        DIRECT = 1,
        MANUAL,
        PAC,
        AUTODETECT,
        SYSTEM,
        UNKNOWN
    };

    explicit Proxy(PROXY_TYPE proxyType);
    ~Proxy();

    void setProxyType(PROXY_TYPE proxyType);
    void setAutoconfigUrl(QString url);
    void setSocksUsername(QString socksUsername);
    void setSocksPassword(QString socksPassword);

    int     proxyType();
    QString autoconfigUrl();
    QString socksUsername();
    QString socksPassword();

    static QString toString(int type);
    static PROXY_TYPE toProxyType(QString text);

protected:

    PROXY_TYPE m_proxyType;
    QString    m_proxyAutoconfigUrl;
    QString    m_socksUsername;
    QString    m_socksPassword;

};

#endif // PROXY_H
