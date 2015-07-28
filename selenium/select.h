#ifndef SELECT_H
#define SELECT_H

#include "webelement.h"
#include "webdriverexception.h"

class By;
class Select
{
    typedef QList<WebElement*> SOptions;

public:

    Select(WebElement* element);

    bool isMultiple();

    SOptions     options();
    SOptions     allSelectedOptions();
    WebElement*  firstSelectedOption();

    void selectByVisibleText(QString text);
    void selectByIndex(int index);
    void selectByValue(QString value);

    void deselectAll();
    void deselectByValue(QString value);
    void deselectByIndex(int index);
    void deselectByVisibleText(QString text);

private:

    WebElement* m_element;
    bool m_isMulti;

    void setSelected(WebElement* option);
};

#endif // SELECT_H
