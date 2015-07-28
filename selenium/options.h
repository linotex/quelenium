#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>

#include "timeouts.h"
#include "window.h"
#include "cookie.h"
#include "imehandler.h"
#include "logs.h"

class WebDriver;
class WebDriverHub;

class Options
{
public:
    explicit Options(WebDriver* driver, WebDriverHub* hub) : m_driver(driver), m_hub(hub) {}

    void deleteAllCookies();
    QList<Cookie *> cookies();
    Cookie* cookieNamed(QString name);
    void addCookie(Cookie* cookie);
    void deleteCookie(Cookie* cookie);
    void deleteCookieNamed(QString name);

    Timeouts* timeouts();
    Window* window();
    ImeHandler* ime();
    Logs* logs();

private:
    WebDriver* m_driver;
    WebDriverHub* m_hub;
};

#endif // OPTIONS_H
