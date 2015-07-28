#ifndef WEBELEMENT_H
#define WEBELEMENT_H

#include <QObject>

class WebDriverHub;
class By;

class WebElement
{
public:
    WebElement(QString id, WebDriverHub* hub);
    virtual ~WebElement() {}

    void click();
    void submit();
    void sendKeys(QString text);
    QString text();
    QString tagName();
    void clear();
    bool isSelected();
    bool isEnabled();
    bool isDisplayed();
    QString attribute(QString name);
    bool equals(QString otherElementId);
    QPoint location();
    QSize size();
    QString cssValue(QString cssProperty);
    WebElement* findElement(By *by);
    QList<WebElement*> findElements(By *by);

    QString id() { return m_id; }

private:
    QString m_id;
    WebDriverHub* m_hub;
};

#endif // WEBELEMENT_H
