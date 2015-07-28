#ifndef ACTIONS_H
#define ACTIONS_H

#include "mouse.h"
#include "touch.h"

class WebDriverHub;
class Actions
{
public:
    explicit Actions(WebDriverHub* hub) : m_hub(hub) {}

    Mouse* mouse();
    Touch* touch();

private:
    WebDriverHub* m_hub;
};

#endif // ACTIONS_H
