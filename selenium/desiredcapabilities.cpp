#include "desiredcapabilities.h"

DesiredCapabilities::DesiredCapabilities(Browser::Type browser)
{
    setTakesScreenshot(true);
    setHandlesAlerts(true);
    setCssSelectorsEnabled(true);

    m_proxy = 0;

    setBrowser(browser);
}

QVariant DesiredCapabilities::property(QString property)
{
    if(!m_properties.value(property).isNull()) {
        return m_properties.value(property);
    }

    qDebug()<<"Property " << property << " is not initialize!";
    return QVariant();
}

void DesiredCapabilities::setProperty(QString property, QString value)
{
    m_properties.insert(property, QJsonValue(value));
}

void DesiredCapabilities::setProperty(QString property, bool value)
{
    m_properties.insert(property, QJsonValue(value));
}

void DesiredCapabilities::setProperty(QString property, int value)
{
    m_properties.insert(property, QJsonValue(value));
}
