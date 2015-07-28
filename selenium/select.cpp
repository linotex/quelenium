#include "select.h"
#include "by.h"

Select::Select(WebElement* element) :
    m_element(element)
{
    QString tagName = m_element->tagName();

    if(tagName.isEmpty() || tagName != "select") {
        WebDriverException::throwException("Element is not select");
    }

    QString value = m_element->attribute("multiple");

    m_isMulti = !value.isEmpty() && (value != "false");
}

bool Select::isMultiple()
{
    return m_isMulti;
}

Select::SOptions Select::options()
{
    return m_element->findElements(By::tagName("option"));
}

Select::SOptions Select::allSelectedOptions()
{
    SOptions selectedOptions;
    SOptions opts = options();

    for(int i = 0; i < opts.size(); i++) {
        if(opts.at(i)->isSelected()) {
            selectedOptions.push_back(opts.at(i));
        }
    }

    return selectedOptions;
}

WebElement* Select::firstSelectedOption()
{
    SOptions opts = options();

    WebElement* option = 0;

    for(int i = 0; opts.size(); i++) {
        if(opts.at(i)->isSelected()) {
            option = opts.at(i);
            break;
        }
    }

    if(option == 0) {
        WebDriverException::throwException("No options are selected", ResponseStatusCodes::NO_SUCH_ELEMENT);
    }

    return option;
}

void Select::selectByVisibleText(QString text)
{
    SOptions opts = options();
    bool matched = false;

    for(int i = 0; i < opts.size(); i++) {
        if(opts.at(i)->text() == text) {

            setSelected(opts.at(i));            

            if(!isMultiple()) {
                return;
            }

            matched = true;
        }
    }

    if(!matched) {
        WebDriverException::throwException("Cannot locate option with text: " + text, ResponseStatusCodes::NO_SUCH_ELEMENT);
    }
}

void Select::selectByIndex(int index)
{
    QString v = QString::number(index);
    bool matched = false;

    SOptions opts = options();

    for(int i = 0; i < opts.size(); i++) {
        if(opts.at(i)->attribute("index") == v) {

            setSelected(opts.at(i));

            if(!isMultiple()) {
                return;
            }

            matched = true;
        }
    }

    if(!matched) {
        WebDriverException::throwException("Cannot locate option with index: " + v, ResponseStatusCodes::NO_SUCH_ELEMENT);
    }
}

void Select::selectByValue(QString value)
{
    SOptions opts = m_element->findElements(By::xpath(".//option[@value = '" + value + "']"));
    bool matched = false;

    for(int i = 0; i < opts.size(); i++) {

        setSelected(opts.at(i));


        if(!isMultiple()) {
            return;
        }

        matched = true;
    }

    if(!matched) {
        WebDriverException::throwException("Cannot locate option with value: " + value, ResponseStatusCodes::NO_SUCH_ELEMENT);
    }
}

void Select::deselectAll()
{
    if(!isMultiple()) {
        WebDriverException::throwException("You may only deselect all options of a multi-select");
        return;
    }

    SOptions opts = options();
    for(int i = 0; i < opts.size(); i++) {

        if(opts.at(i)->isSelected()) {
            opts.at(i)->click();
        }
    }

}

void Select::deselectByValue(QString value)
{
    if(!isMultiple()) {
        WebDriverException::throwException("You may only deselect option of a multi-select");
        return;
    }

    SOptions opts = m_element->findElements(By::xpath(".//option[@value = '" + value + "']"));

    for(int i = 0; i < opts.size(); i++) {

        if(opts.at(i)->isSelected()) {
            opts.at(i)->click();
        }
    }
}

void Select::deselectByIndex(int index)
{
    if(!isMultiple()) {
        WebDriverException::throwException("You may only deselect option of a multi-select");
        return;
    }

    QString v = QString::number(index);

    SOptions opts = options();

    for(int i = 0; i < opts.size(); i++) {
        if(opts.at(i)->attribute("index") == v && opts.at(i)->isSelected()) {
            opts.at(i)->click();
        }
    }
}

void Select::deselectByVisibleText(QString text)
{
    if(!isMultiple()) {
        WebDriverException::throwException("You may only deselect option of a multi-select");
        return;
    }

    SOptions opts = options();

    for(int i = 0; i < opts.size(); i++) {
        if(opts.at(i)->text() == text && opts.at(i)->isSelected()) {
            opts.at(i)->click();
        }
    }
}

void Select::setSelected(WebElement *option)
{
    if(!option->isSelected()) {
        option->click();
    }
}
