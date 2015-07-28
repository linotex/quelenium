#ifndef TARGETLOCATOR_H
#define TARGETLOCATOR_H

#include <QObject>

#include "alert.h"

class WebDriverHub;
class WebElement;
class TargetLocator
{
public:
    explicit TargetLocator(WebDriverHub* hub) : m_hub(hub) {}

    void frame(QString id);
    void frame(int id);
    void frame(WebElement *element);
    void parentFrame();
    void defaultContent();
    WebElement* activeElement();
    void window(QString handle);
    Alert* alert();

private:
    WebDriverHub* m_hub;
};

#endif // TARGETLOCATOR_H
