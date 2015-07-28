#include "by.h"

By::By(QString strategy, QString value):
    m_strategy(strategy),
    m_value(value)
{
}

By* By::id(QString id)
{
    return new By("id", id);
}

By* By::linkText(QString linkText)
{
    return new By("link text", linkText);
}

By* By::partialLinkText(QString linkText)
{
    return new By("partial link text", linkText);
}

By* By::name(QString name)
{
    return new By("name", name);
}

By* By::tagName(QString tagName)
{
    return new By("tag name", tagName);
}

By* By::xpath(QString xpath)
{
    return new By("xpath", xpath);
}

By* By::className(QString className)
{
    return new By("class name", className);
}

By* By::cssSelector(QString cssSelector)
{
    return new By("css selector", cssSelector);
}

QString By::strategy()
{
    return m_strategy;
}

QString By::value()
{
    return m_value;
}
