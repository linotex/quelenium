#include "navigation.h"
#include "webdriver.h"
#include "webdriverhub.h"

void Navigation::forward()
{
    m_hub->forward();
}

void Navigation::back()
{
    m_hub->back();
}

void Navigation::refresh()
{
    m_hub->refresh();
}

void Navigation::to(const QString& url)
{
    m_driver->get(url);
}

void Navigation::to(QUrl url)
{
    m_driver->get(url);
}
