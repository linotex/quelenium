#include "alert.h"
#include "webdriverhub.h"

void Alert::accept()
{
    m_hub->acceptAlert();
}

void Alert::dismiss()
{
    m_hub->dismissAlert();
}

QString Alert::text()
{
    return m_hub->getAlertText();
}

void Alert::sendKeys(QString text)
{
    m_hub->setAlertText(text);
}
