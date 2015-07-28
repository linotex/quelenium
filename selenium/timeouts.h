#ifndef TIMEOUTS_H
#define TIMEOUTS_H

#include <QObject>

class WebDriver;
class WebDriverHub;

class Timeouts
{
public:

    explicit Timeouts(WebDriver *driver, WebDriverHub *hub) : m_driver(driver), m_hub(hub) {}

    enum TIMEOUT_TYPE {
        SCRIPT,
        IMPLICIT,
        PAGE_LOAD
    };

    static QString toString(int type);

    Timeouts* setScriptTimeout(long ms);
    Timeouts* pageLoadTimeout(long ms);
    Timeouts* implicitlyWait(long ms);

private:
    WebDriver*    m_driver;
    WebDriverHub* m_hub;
};

#endif // TIMEOUTS_H
