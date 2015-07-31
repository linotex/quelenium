#ifndef SERVER_H
#define SERVER_H

#include <QJsonObject>
#include <QDebug>

// Server build
struct Build {

    public:
        Build(QJsonObject build) : m_build(build) {}

        QString revision(){ return m_build["revision"].toString(); }
        QString time()    { return m_build["time"].toString();     }
        QString version() { return m_build["version"].toString();  }

    private:
        QJsonObject m_build;
};

// Server OS
struct Os {

    public:
        Os(QJsonObject os) : m_os(os) {}

        QString arch()    { return m_os["arch"].toString(); }
        QString name()    { return m_os["name"].toString(); }
        QString version() { return m_os["version"].toString(); }

    private:
        QJsonObject m_os;
};

// Server Java
struct Java {

public:
    Java(QJsonObject java) : m_java(java) {}

    QString version() { return m_java["version"].toString(); }

private:
    QJsonObject m_java;
};

// Server status
struct Status
{
    public:
        Status(Os* os, Build* build, Java* java = 0) : m_os(os), m_build(build), m_java(java)
        {}

        Os*    os()    { return m_os; }
        Build* build() { return m_build; }
        Java*  java()  { return m_java; }

    private:
        Os*    m_os;
        Build* m_build;
        Java* m_java;
};

class WebDriverHub;
class Server
{
public:
    explicit Server(WebDriverHub* hub);

    Status* status();

private:
    WebDriverHub* m_hub;
};

#endif // SERVER_H
