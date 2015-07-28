#ifndef ALERT_H
#define ALERT_H

#include <QObject>

class WebDriverHub;
class Alert
{
public:
    explicit Alert(WebDriverHub *hub) : m_hub(hub) {}

    void dismiss();
    void accept();
    QString text();
    void sendKeys(QString text);

private:
    WebDriverHub* m_hub;
};

#endif // ALERT_H
