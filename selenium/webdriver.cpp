#include "webdriver.h"
#include "webdriverhub.h"

WebDriver::WebDriver(QString host, int port, Browser::Type browser, QString url)
{
    DesiredCapabilities *dc = new DesiredCapabilities(browser);

    init(host, port, dc, url);
}

WebDriver::WebDriver(QString host, int port, DesiredCapabilities* desiredCapabilities, QString url)
{
    init(host, port, desiredCapabilities, url);
}

WebDriver::WebDriver(QString host, int port, Browser::Type browser)
{
    DesiredCapabilities *dc = new DesiredCapabilities(browser);
    init(host, port, dc);
}

WebDriver::WebDriver(QString host, int port, DesiredCapabilities *desiredCapabilities)
{
    init(host, port, desiredCapabilities);
}

WebDriver::~WebDriver()
{
    qDebug()<<"I am destructor WebDriver!";
}

/**
 * Init WebDriverHub
 */
void WebDriver::init(QString host, int port, DesiredCapabilities *dc, QString url)
{
    init(host, port, dc);
    get(url);
}

void WebDriver::init(QString host, int port, DesiredCapabilities *dc)
{
    m_wdh =  new WebDriverHub(host, port, dc);
    m_wdh->startSession();
}

WebDriverHub* WebDriver::getHub()
{
    return m_wdh;
}

/**
 * Get driver capabilities
 */
DesiredCapabilities* WebDriver::capabilities(bool refresh)
{
    return m_wdh->capabilities(refresh);
}

void WebDriver::get(const QString &url)
{
    m_wdh->setUrl(url);
}

void WebDriver::get(QUrl url)
{
    get(url.url());
}

void WebDriver::quit()
{
    m_wdh->deleteSession();
}

void WebDriver::close()
{
    m_wdh->deleteWindow();
}

QString WebDriver::currentUrl()
{
    return m_wdh->getUrl();
}
QString WebDriver::title()
{
    return m_wdh->getTitle();
}

QString WebDriver::pageSource()
{
    return m_wdh->getSource();
}

QString WebDriver::windowHandle()
{
    return m_wdh->getWindowHandle();
}

QStringList WebDriver::windowHandles()
{
    return m_wdh->getWindowHandles();
}

WebElement* WebDriver::findElement(By *by)
{
    QString id = m_wdh->getElement(by->strategy(), by->value());
    return new WebElement(id, m_wdh);
}

QList<WebElement*> WebDriver::findElements(By *by)
{
    QStringList ids = m_wdh->getElements(by->strategy(), by->value());
    QList<WebElement*> elements;
    for(int i = 0; i < ids.size(); i++) {
        elements.push_back(new WebElement(ids.at(i), m_wdh));
    }

    return elements;
}

Navigation* WebDriver::navigate()
{
    return new Navigation(this, getHub());
}

Options* WebDriver::manage()
{
    return new Options(this, getHub());
}

TargetLocator* WebDriver::switchTo()
{
    return new TargetLocator(getHub());
}

Actions* WebDriver::actions()
{
    return new Actions(getHub());
}

Server* WebDriver::server()
{
    return new Server(getHub());
}
