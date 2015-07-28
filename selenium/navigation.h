#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include <QUrl>

class WebDriver;
class WebDriverHub;
class Navigation
{
public:
    explicit Navigation(WebDriver* driver, WebDriverHub* hub) : m_driver(driver), m_hub(hub) {}

    void forward();
    void back();
    void refresh();

    void to(const QString& url);
    void to(QUrl url);

private:
    WebDriver* m_driver;
    WebDriverHub* m_hub;
};

#endif // NAVIGATION_H
