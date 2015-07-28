#include "mouse.h"
#include "webdriverhub.h"

void Mouse::moveTo(WebElement *element)
{
    m_hub->mouseMoveTo(element->id());
}

void Mouse::moveTo(int x, int y)
{
    m_hub->mouseMoveTo(x, y);
}

void Mouse::moveTo(QPoint position)
{
    moveTo(position.x(), position.y());
}

void Mouse::click(int button)
{
    m_hub->mouseClick(button);
}

void Mouse::buttondown(int button)
{
    m_hub->mouseButtonDown(button);
}

void Mouse::buttonup(int button)
{
    m_hub->mouseButtonUp(button);
}

void Mouse::doubleclick()
{
    m_hub->mouseDoubleClick();
}
