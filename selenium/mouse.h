#ifndef MOUSE_H
#define MOUSE_H

#include <QObject>

#include "webelement.h"

class WebDriverHub;
class Mouse
{
public:
    explicit Mouse(WebDriverHub* hub) : m_hub(hub) {}

    enum MOUSE_BUTTONS {
        LEFT   = 0,
        MIDDLE = 1,
        RIGHT  = 2
    };

    void moveTo(WebElement* element);
    void moveTo(int x, int y);
    void moveTo(QPoint position);
    void click(int button = Mouse::LEFT);

    //TODO Not work in Selenium Server Standalone 2.x. Only default value (LEFT)
    void buttondown(int button = Mouse::LEFT);


    void buttonup(int button = Mouse::LEFT);
    void doubleclick();

private:
    WebDriverHub* m_hub;
};

#endif // MOUSE_H
