#include "webelement.h"
#include "webdriverhub.h"
#include "by.h"

WebElement::WebElement(QString id, WebDriverHub *hub):
    m_id(id),
    m_hub(hub)
{
}

void WebElement::click()
{
    m_hub->elementClick(m_id);
}

void WebElement::submit()
{
    m_hub->elementSubmit(m_id);
}

QString WebElement::text()
{
    return m_hub->getElementText(m_id);
}

void WebElement::sendKeys(QString text)
{
    m_hub->elementValue(m_id, QStringList(text));
}

QString WebElement::tagName()
{
    return m_hub->getElementTagName(m_id);
}

void WebElement::clear()
{
    m_hub->elementClear(m_id);
}

bool WebElement::isSelected()
{
    return m_hub->isElementSelected(m_id);
}

bool WebElement::isEnabled()
{
    return m_hub->isElementEnabled(m_id);
}

QString WebElement::attribute(QString name)
{
    return m_hub->getElementAttributeValue(m_id, name);
}

bool WebElement::equals(QString otherElementId)
{
    return m_hub->elementEqualsOther(m_id, otherElementId);
}

bool WebElement::isDisplayed()
{
    return m_hub->isElementDisplayed(m_id);
}

QPoint WebElement::location()
{
    return m_hub->getElementLocation(m_id);
}

QSize WebElement::size()
{
    return m_hub->getElementSize(m_id);
}

QString WebElement::cssValue(QString cssProperty)
{
    return m_hub->getElementCssPropertyValue(m_id, cssProperty);
}

WebElement* WebElement::findElement(By *by)
{
    QString id = m_hub->getElementByElement(m_id, by->strategy(), by->value());
    return new WebElement(id, m_hub);
}

QList<WebElement*> WebElement::findElements(By *by)
{
    QStringList ids = m_hub->getElementsByElement(m_id, by->strategy(), by->value());
    QList<WebElement*> elements;
    for(int i = 0; i < ids.size(); i++) {
        elements.push_back(new WebElement(ids.at(i), m_hub));
    }

    return elements;
}
