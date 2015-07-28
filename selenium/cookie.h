#ifndef COOKIE_H
#define COOKIE_H

#include <QObject>
#include <QDateTime>

class Cookie
{
public:
    explicit Cookie(QString name, QString value);
    Cookie(QString name, QString value, QString path);
    Cookie(QString name, QString value, QString path, QDateTime expiry);
    Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry);
    Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry, bool isSecure);
    Cookie(QString name, QString value, QString path, QString domain, QDateTime expiry, bool isSecure, bool isHttpOnly);

    QString name();
    QString value();
    QString path();
    QString domain();
    bool    isSecure();
    bool    isHttpOnly();
    QDateTime expiry();
    int     hashCode();

    void setHashCode(int hashCode);

private:

    void init(QString name, QString value, QString path, QString domain, bool secure, bool httpOnly, QDateTime expiry);

    void setName(QString name);
    void setValue(QString value);
    void setPath(QString path);
    void setDomain(QString domain);
    void setSecure(bool secure);
    void setHttpOnly(bool only);
    void setExpiry(QDateTime expiry);

    QDateTime getDefaultTime();

    QString m_name;
    QString m_value;
    QString m_path;
    QString m_domain;
    bool    m_secure;
    bool    m_httpOnly;
    QDateTime   m_expiry;
    int     m_hashCode;
};

#endif // COOKIE_H
