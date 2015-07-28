#include "imehandler.h"
#include "webdriverhub.h"

ImeHandler::ImeHandler(WebDriverHub *hub):
    m_hub(hub)
{
}

QStringList ImeHandler::availableEngines()
{
    return m_hub->getImeAvailableEngines();
}

QString ImeHandler::activeEngine()
{
    return m_hub->getImeActiveEngine();
}

void ImeHandler::activateEngine(QString engine)
{
    m_hub->imeActivate(engine);
}

void ImeHandler::deactivate()
{
    m_hub->imeDeactivate();
}

bool ImeHandler::isActivated()
{
    return m_hub->isImeActivated();
}
