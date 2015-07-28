#ifndef TOUCH_H
#define TOUCH_H

#include "webelement.h"

class WebDriverHub;
class Touch
{
public:
    explicit Touch(WebDriverHub* hub) : m_hub(hub) {}

    void click(WebElement* element);
    void down(int x, int y);
    void down(QPoint pos);
    void down(WebElement* element);
    void up(int x, int y);
    void up(QPoint pos);
    void up(WebElement* element);
    void move(int x, int y);
    void move(QPoint pos);
    void move(WebElement* element);
    void scroll(WebElement* element, int xoffset, int yoffset);
    void scroll(int xoffset, int yoffset);
    void doubleclick(WebElement* element);
    void longclick(WebElement* element);
    void flick(WebElement* element, int xoffset, int yoffset, int speed);
    void flick(int xspeed, int yspeed);

private:
    WebDriverHub* m_hub;
};

#endif // TOUCH_H
