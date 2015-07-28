#include "touch.h"
#include "webdriverhub.h"

void Touch::click(WebElement *element)
{
    m_hub->touchClick(element->id());
}

void Touch::down(int x, int y)
{
    m_hub->touchDown(x, y);
}

void Touch::down(QPoint pos)
{
    down(pos.x(), pos.y());
}

void Touch::down(WebElement* element)
{
    QPoint p = element->location();
    down(p.x(), p.y());
}

void Touch::up(int x, int y)
{
    m_hub->touchUp(x, y);
}

void Touch::up(QPoint pos)
{
    up(pos.x(), pos.y());
}

void Touch::up(WebElement* element)
{
    QPoint p = element->location();
    up(p.x(), p.y());
}

void Touch::move(int x, int y)
{
    m_hub->touchMove(x, y);
}

void Touch::move(QPoint pos)
{
    move(pos.x(), pos.y());
}

void Touch::move(WebElement* element)
{
    QPoint p = element->location();
    move(p.x(), p.y());
}

void Touch::scroll(WebElement* element, int xoffset, int yoffset)
{
    m_hub->touchScroll(element->id(), xoffset, yoffset);
}

void Touch::scroll(int xoffset, int yoffset)
{
    m_hub->touchScroll(xoffset, yoffset);
}

void Touch::doubleclick(WebElement* element)
{
    m_hub->touchDoubleClick(element->id());
}

void Touch::longclick(WebElement* element)
{
    m_hub->touchLongClick(element->id());
}

void Touch::flick(WebElement* element, int xoffset, int yoffset, int speed)
{
    m_hub->touchFlick(element->id(), xoffset, yoffset, speed);
}

void Touch::flick(int xspeed, int yspeed)
{
    m_hub->touchFlick(xspeed, yspeed);
}
