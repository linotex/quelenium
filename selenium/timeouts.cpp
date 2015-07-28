#include "timeouts.h"
#include "webdriver.h"
#include "webdriverhub.h"

Timeouts* Timeouts::setScriptTimeout(long ms)
{
    m_hub->setTimeouts(toString(SCRIPT), ms);
    return this;
}

Timeouts* Timeouts::pageLoadTimeout(long ms)
{
    m_hub->setTimeouts(toString(PAGE_LOAD), ms);
    return this;
}

Timeouts* Timeouts::implicitlyWait(long ms)
{
    m_hub->setTimeouts(toString(IMPLICIT), ms);
    return this;
}

QString Timeouts::toString(int type)
{
    QString name;

    switch(type) {

    case SCRIPT:
        name = "script";
        break;

    case IMPLICIT:
        name = "implicit";
        break;

    case PAGE_LOAD:
        name = "page load";
        break;

    default:
        name = "Unknown Type";
        break;
    }

    return name;
}
