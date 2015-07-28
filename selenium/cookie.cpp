#include "cookie.h"

Cookie::Cookie(QString name, QString value)
{
    init(name, value, "", "", false, false, getDefaultTime());
}

Cookie::Cookie(QString name, QString value, QString path)
{
    init(name, value, path, "", false, false, getDefaultTime());
}

Cookie::Cookie(QString name, QString value, QString path, QDateTime expiry)
{
    init(name, value, path, "", false, false, expiry);
}

Cookie::Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry)
{
    init(name, value, path, domain, false, false, expiry);
}

Cookie::Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry, bool isSecure)
{
    init(name, value, path, domain, isSecure, false, expiry);
}

Cookie::Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry, bool isSecure, bool isHttpOnly)
{
    init(name, value, path, domain, isSecure, isHttpOnly, expiry);
}

void Cookie::init(QString name, QString value, QString path, QString domain, bool secure, bool httpOnly, QDateTime expiry)
{
    setName(name);
    setValue(value);
    setPath(path);
    setDomain(domain);
    setSecure(secure);
    setHttpOnly(httpOnly);
    setExpiry(expiry);
    setHashCode(0);
}

QString Cookie::name()
{
    return m_name;
}

QString Cookie::value()
{
    return m_value;
}

QString Cookie::path()
{
    return m_path;
}

QString Cookie::domain()
{
    return m_domain;
}

bool Cookie::isSecure()
{
    return m_secure;
}

bool Cookie::isHttpOnly()
{
    return m_httpOnly;
}

QDateTime Cookie::expiry()
{
    return m_expiry;
}

int Cookie::hashCode()
{
    return m_hashCode;
}

void Cookie::setName(QString name)
{
    m_name = name;
}

void Cookie::setValue(QString value)
{
    m_value = value;
}

void Cookie::setPath(QString path)
{
    m_path = path;
}

void Cookie::setDomain(QString domain)
{
    m_domain = domain;
}

void Cookie::setSecure(bool secure)
{
    m_secure = secure;
}

void Cookie::setHttpOnly(bool only)
{
    m_httpOnly = only;
}


void Cookie::setExpiry(QDateTime expiry)
{
    m_expiry = expiry;
}

void Cookie::setHashCode(int hashCode)
{
    m_hashCode = hashCode;
}

QDateTime Cookie::getDefaultTime()
{
    return QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t() + 3600);
}
