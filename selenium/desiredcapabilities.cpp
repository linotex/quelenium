#include "desiredcapabilities.h"

DesiredCapabilities::DesiredCapabilities(BrowserType::BROWSER_TYPE browser)
{
    m_takesScreenshot     = true;
    m_handlesAlerts       = true;
    m_cssSelectorsEnabled = true;
    m_javascriptEnabled   = true;

    m_proxy = 0;

    setBrowser(browser);
}

QVariant DesiredCapabilities::getProperty(QString property, QVariant value)
{
    if(m_properties.contains(property)) {
        return value;
    }

    qDebug()<<"Property "<<property<<" is not initialize!";
    return QVariant();
}


