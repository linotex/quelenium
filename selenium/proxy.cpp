#include "proxy.h"

Proxy::Proxy(PROXY_TYPE proxyType)
{
    setProxyType(proxyType);
}

Proxy::~Proxy()
{
    qDebug()<<"I am destructor ProxyObject!";
}

void Proxy::setProxyType(PROXY_TYPE proxyType)
{
    m_proxyType = proxyType;
}

int Proxy::proxyType()
{
    return m_proxyType;
}

/**
 * Required for PAC proxy type, Ignored otherwise
 */
void Proxy::setAutoconfigUrl(QString url)
{
    if(m_proxyType == PAC) {

        m_proxyAutoconfigUrl = url;

    } else {
        qDebug()<<"Required only for PAC proxy type";
    }
}

QString Proxy::autoconfigUrl()
{
    return m_proxyAutoconfigUrl;
}

/**
 * Optional only MANUAL proxy type
 */
void Proxy::setSocksUsername(QString socksUsername)
{
    if(m_proxyType == Proxy::MANUAL) {
        m_socksUsername = socksUsername;
    } else {
        qDebug()<<"Optional only MANUAL proxy type";
    }
}

QString Proxy::socksUsername()
{
    return m_socksUsername;
}

void Proxy::setSocksPassword(QString socksPassword)
{
    if(m_proxyType == Proxy::MANUAL) {
        m_socksPassword = socksPassword;
    } else {
        qDebug()<<"Optional only MANUAL proxy type";
    }
}

QString Proxy::socksPassword()
{
    return m_socksPassword;
}

QString Proxy::toString(int type)
{
    QString name;

    switch (type) {

    case DIRECT:
        name = "direct";
        break;

    case MANUAL:
        name = "manual";
        break;

    case PAC:
        name = "pac";
        break;

    case AUTODETECT:
        name = "autodetect";
        break;

    case SYSTEM:
        name = "system";
        break;

    default:
        name = "unknown";
        break;
    }

    return name;
}

Proxy::PROXY_TYPE Proxy::toProxyType(QString text)
{
    PROXY_TYPE proxy = UNKNOWN;

    text = text.toLower();

    if(text == "direct") {
        proxy = DIRECT;

    } else if(text == "manual") {
        proxy = MANUAL;

    } else if(text == "pac") {
        proxy = PAC;

    } else if(text == "autodetect") {
        proxy = AUTODETECT;

    } else if(text == "system") {
        proxy = SYSTEM;
    }

    return proxy;
}

