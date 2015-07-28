#ifndef BY_H
#define BY_H

#include <QObject>

class By
{
    By(QString strategy, QString value);
public:
    static By* id(QString id);
    static By* linkText(QString linkText);
    static By* partialLinkText(QString linkText);
    static By* name(QString name);
    static By* tagName(QString tagName);
    static By* xpath(QString xpath);
    static By* className(QString className);
    static By* cssSelector(QString cssSelector);

    QString strategy();
    QString value();

private:
    QString m_strategy;
    QString m_value;
};

#endif // BY_H
