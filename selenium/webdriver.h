   #ifndef WEBDRIVER_H
#define WEBDRIVER_H

#include <QObject>
#include <QUrl>

#include "browsertype.h"
#include "desiredcapabilities.h"
#include "navigation.h"
#include "options.h"
#include "webelement.h"
#include "select.h"
#include "by.h"
#include "targetlocator.h"
#include "actions.h"

class WebDriverHub;

class WebDriver
{

public:
    explicit WebDriver(QString host, QString port, BrowserType::BROWSER_TYPE browser, QString url);
    WebDriver(QString host, QString port, DesiredCapabilities* desiredCapabilities, QString url);

    WebDriver(QString host, QString port, BrowserType::BROWSER_TYPE browser);
    WebDriver(QString host, QString port, DesiredCapabilities* desiredCapabilities);

    ~WebDriver();

    DesiredCapabilities* capabilities(bool refresh = false) ;

    void get(const QString& url);
    void get(QUrl url);

    void quit();
    void close();

    QString currentUrl();
    QString title();
    QString pageSource();
    QString windowHandle();
    QStringList windowHandles();

    WebElement* findElement(By* by);
    QList<WebElement*> findElements(By* by);

    Navigation*     navigate();
    Options*        manage();
    TargetLocator*  swichTo();
    Actions*        actions();

protected:
    void init(QString host, QString port, DesiredCapabilities* dc, QString url);
    void init(QString host, QString port, DesiredCapabilities* dc);
    WebDriverHub* getHub();

private:
    WebDriverHub* m_wdh;
};

#endif // WEBDRIVER_H
