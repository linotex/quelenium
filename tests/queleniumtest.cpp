#include <QTest>
#include <QCoreApplication>

#include "queleniumtest.h"

QueleniumTest::QueleniumTest()
{
}

/**
 * Before Test Case
 */
void QueleniumTest::init()
{
    //This local web server for easy testing
    m_testUrl = "quelenium.local.test";
    m_host    = "192.168.53.147";
    m_port    = 4444;

    Proxy *proxy = new Proxy(Proxy::DIRECT);
    DesiredCapabilities *cap = new DesiredCapabilities(Browser::FIREFOX);
    cap->setProxy(proxy);

    try {
        driver = new WebDriver(m_host, m_port, cap);
    } catch (WebDriverException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/**
 * After Test Case
 */
void QueleniumTest::cleanup()
{
    driver->quit();
}

/**
 * Check all cookies
 */
int QueleniumTest::checkAllCookies()
{
    QList<Cookie*> list = driver->manage()->cookies();

    int result = 0;
    for(int i = 0; i < list.size(); i++) {

        result += ((list.at(i)->name() == "CookieTest1") ? 1 : 0);
        result += ((list.at(i)->name() == "CookieTest2") ? 1 : 0);
        result += ((list.at(i)->name() == "CookieTest3") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST1") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST2") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST3") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST4") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST5") ? 1 : 0);
        result += ((list.at(i)->name() == "TEST6") ? 1 : 0);
    }

    return result;
}

void QueleniumTest::compareCookie(Cookie *original, Cookie *tested, QString defaultPath, QString defaultDomain)
{
    qDebug()<<original->name()<<" >> "<<tested->name();

    QCOMPARE(original->name(), tested->name());
    QCOMPARE(original->value(), tested->value());
    QCOMPARE(defaultPath.isEmpty() ? original->path(): defaultPath, tested->path());
    QCOMPARE(defaultDomain.isEmpty() ? original->domain(): defaultDomain, tested->domain());
    QCOMPARE(original->expiry(), tested->expiry());
    QCOMPARE(original->isHttpOnly(), tested->isHttpOnly());
    QCOMPARE(original->isSecure(), tested->isSecure());
}

void QueleniumTest::notFoundWindow()
{
    QString windowHandle = driver->windowHandle();
    driver->switchTo()->window(windowHandle);

    WebElement* elm = driver->findElement(By::tagName("body"));
    elm->sendKeys(Keys::CONTROL + "n");//open new window

    driver->close();
}

void QueleniumTest::mouseButtonEvents(QString text, int event, int button)
{
    driver->get(m_testUrl + "/mouse/click.html");
    driver->manage()->window()->maximize();

    //TODO change to WebDriver Wait in future
    QThread::sleep(2);

    WebElement* elm1 = driver->findElement(By::id("element1"));
    driver->actions()->mouse()->moveTo(elm1);

    switch (event) {
    case CLICK:
        if(button == -1) {
            driver->actions()->mouse()->click();
        } else {
            driver->actions()->mouse()->click(button);
        }
        break;

    case DOWN:
        if(button == -1) {
            driver->actions()->mouse()->buttondown();
        } else {
            driver->actions()->mouse()->buttondown(button);

        }
        break;

    case UP:
        if(button == -1) {
            driver->actions()->mouse()->buttondown();
            driver->actions()->mouse()->buttonup();
        } else {
            driver->actions()->mouse()->buttondown(button);
            driver->actions()->mouse()->buttonup(button);

        }
        break;

    case CLICK_DOUBLE:
        driver->actions()->mouse()->doubleclick();
        break;
    default:
        QFAIL(QString("Unknown button event").toUtf8());
        break;
    }

    WebElement* elm2 = driver->findElement(By::id("element2"));
    QCOMPARE(elm2->text(), QString(text));
}

/******** DRIVER TESTS ************************************************************************************************/
/**
 * void driver->get(QString)
 */
void QueleniumTest::getCase1()
{
    driver->get(m_testUrl);

    QCOMPARE(driver->currentUrl(), "http://" + m_testUrl + "/");
}

/**
 * void driver->get(QUrl)
 */
void QueleniumTest::getCase2()
{
    QUrl url(m_testUrl);
    driver->get(url);

    QCOMPARE(driver->currentUrl(), "http://" + m_testUrl + "/");
}

/**
 * QString driver->title()
 */
void QueleniumTest::titleCase1()
{
    driver->get(m_testUrl);

    QCOMPARE(driver->title(), QString("Quelenium Local Test"));
}

/**
 * QString driver->pageSource()
 */
void QueleniumTest::pageSourceCase1()
{
    driver->get(m_testUrl + "/page_source/case1.html");
    QString source = driver->pageSource();

    QVERIFY(!source.isNull());
    QVERIFY(!source.isEmpty());
    QVERIFY(source.contains("<head><title>Page Source - Case 1</title></head><body>Test</body></html>"));
}

/**
 * QString driver->windowHandle()
 */
void QueleniumTest::windowHandleCase1()
{
    driver->get(m_testUrl);
    QString handle = driver->windowHandle();

    QVERIFY(!handle.isNull());
    QVERIFY(!handle.isEmpty());    
}

/**
 * QStringList driver->windowHandles()
 */
void QueleniumTest::windowHandlesCase1()
{
    driver->get(m_testUrl);

    QString handle = driver->windowHandle();
    QStringList handles = driver->windowHandles();

    QCOMPARE(handles.size(), 1);
    QVERIFY(handles.contains(handle));
}

/**
 * QStringList driver->windowHandles()
 */
void QueleniumTest::windowHandlesCase2()
{
    driver->get(m_testUrl);

    QString handle = driver->windowHandle();

    WebElement* elm = driver->findElement(By::tagName("body"));
    elm->sendKeys(Keys::CONTROL + "n");
    elm->sendKeys(Keys::CONTROL + "n");
    elm->sendKeys(Keys::CONTROL + "n");

    QStringList handles = driver->windowHandles();

    QCOMPARE(handles.size(), 4);
    QVERIFY(handles.contains(handle));
}

/**
 * WebElement* driver->findElement(By:id)
 */
void QueleniumTest::findElementCase1()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::id("findElementCase1"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("class"), QString("findElementCase1-css-class"));
}

/**
 * WebElement* driver->findElement(By:linkText)
 */
void QueleniumTest::findElementCase2()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::linkText("findElementCase2 link text"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("class"), QString("findElementCase2-css-class"));
    QCOMPARE(elm->text(), QString("findElementCase2 link text"));
}

/**
 * WebElement* driver->findElement(By:partialLinkText)
 */
void QueleniumTest::findElementCase3()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::partialLinkText("Case3 link"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("class"), QString("findElementCase3-css-class"));
    QCOMPARE(elm->text(), QString("findElementCase3 link text"));
}

/**
 * WebElement* driver->findElement(By:name)
 */
void QueleniumTest::findElementCase4()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::name("findElementCase4"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("class"), QString("findElementCase4-css-class"));
}

/**
 * WebElement* driver->findElement(By:tagName)
 */
void QueleniumTest::findElementCase5()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::tagName("input"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("name"), QString("findElementCase5"));
}

/**
 * WebElement* driver->findElement(By:xpath)
 */
void QueleniumTest::findElementCase6()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::xpath("//input[@id='findElementCase6']"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("class"), QString("findElementCase6-css-class"));
}

/**
 * WebElement* driver->findElement(By:className)
 */
void QueleniumTest::findElementCase7()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::className("findElementCase7-css-class"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("id"), QString("findElementCase7"));
}

/**
 * WebElement* driver->findElement(By:cssSelector)
 */
void QueleniumTest::findElementCase8()
{
    driver->get(m_testUrl);

    WebElement* elm = driver->findElement(By::cssSelector("tr#tr-findElementCase8 > td >.findElementCase8-css-class"));

    QVERIFY(!elm->id().isEmpty());
    QCOMPARE(elm->attribute("id"), QString("findElementCase8"));
}

/**
 * QList<WebElement*> driver->findElements(By* by)
 */
void QueleniumTest::findElementsCase1()
{
    driver->get(m_testUrl);

    Elements elms = driver->findElements(By::className("findElementCase9-css-class"));
    QCOMPARE(elms.size(), 3);
}
/******** NAVIGATION TESTS ********************************************************************************************/

/**
 * driver->navigate()->back()
 */
void QueleniumTest::navigateBackCase()
{
    driver->get(m_testUrl);
    driver->get(m_testUrl + "/navigate/back_forward.html");

    driver->navigate()->back();

    QCOMPARE(driver->title(), QString("Quelenium Local Test"));
}

/**
 * void driver->navigate()->forward()
 */
void QueleniumTest::navigateForwardCase()
{
    driver->get(m_testUrl);
    driver->get(m_testUrl + "/navigate/back_forward.html");

    driver->navigate()->back();
    driver->navigate()->forward();

    QCOMPARE(driver->title(), QString("This navigateForwardCase navigateBackCase"));
}

/**
 * void driver->navigate()->refresh()
 */
void QueleniumTest::navigateRefreshCase()
{
    driver->get(m_testUrl + "/navigate/refresh.html");

    int time = driver->findElement(By::id("time"))->text().toInt();

    QThread::sleep(2);

    driver->navigate()->refresh();

    int time2 = driver->findElement(By::id("time"))->text().toInt();

    QVERIFY(time2 - time >= 2);
}

/**
 * void driver->navigate()->to(QString)
 */
void QueleniumTest::navigateToCase1()
{
    driver->navigate()->to(m_testUrl);

    QCOMPARE(driver->title(), QString("Quelenium Local Test"));
}

/**
 * void driver->navigate()->to(QUrl)
 */
void QueleniumTest::navigateToCase2()
{
    QUrl url(m_testUrl+"/404page.html");
    driver->navigate()->to(url);

    QCOMPARE(driver->title(), QString("404"));
}

/******** MANAGE TESTS ************************************************************************************************/

/**
 * QList<Cookie*> driver->manage()->cookies()
 */
void QueleniumTest::manageCookiesCase()
{
    driver->get(m_testUrl + "/cookie/case.html");

    int result = checkAllCookies();

    QCOMPARE(result, 3);
}

/**
 * Cookie* driver-manage()->cookieNamed(QString)
 */
void QueleniumTest::manageCookieNamedCase()
{
    driver->get(m_testUrl + "/cookie/case.html");
    Cookie* cookie = driver->manage()->cookieNamed("CookieTest2");

    QCOMPARE(cookie->name(), QString("CookieTest2"));
    QCOMPARE(cookie->domain(), m_testUrl);
}

/**
 * void driver->manage()->deleteAllCookies()
 */
void QueleniumTest::manageDeleteAllCookiesCase()
{
    driver->get(m_testUrl + "/cookie/case.html");
    driver->manage()->deleteAllCookies();

    QList<Cookie*> list = driver->manage()->cookies();
    QCOMPARE(list.size(), 0);
}

/**
 * void driver->manage()->deleteCookie(Cookie*)
 */
void QueleniumTest::manageDeleteCookieCase()
{
    driver->get(m_testUrl + "/cookie/case.html");

    Cookie* cookie = new Cookie("CookieTest3", "");
    driver->manage()->deleteCookie(cookie);

    int result = checkAllCookies();

    QCOMPARE(result, 2);
}

/**
 * void driver->manage()->deleteCookie(QString)
 */
void QueleniumTest::manageDeleteCookieNamedCase()
{
    driver->get(m_testUrl + "/cookie/case.html");
    driver->manage()->deleteCookieNamed("CookieTest1");

    int result = checkAllCookies();

    QCOMPARE(result, 2);
}

/**
 * void driver->manage()->addCookie(Cookie*)
 */
void QueleniumTest::manageAddCookieCase()
{
    driver->get(m_testUrl + "/cookie/case.html");

    QDateTime expiry = QDateTime::fromString("2020-01-02 03:04:05","yyyy-MM-dd HH:mm:ss");

    //******************************************************************************************************************
    Cookie* cookie1 = new Cookie("TEST1", "VALUE1");
    driver->manage()->addCookie(cookie1);  

    Cookie* cookie11 = driver->manage()->cookieNamed("TEST1");
    compareCookie(cookie1, cookie11, "/", m_testUrl);
    //******************************************************************************************************************

    Cookie* cookie2 = new Cookie("TEST2", "VALUE2", "/");
    driver->manage()->addCookie(cookie2);

    Cookie* cookie21 = driver->manage()->cookieNamed("TEST2");
    compareCookie(cookie2, cookie21, "", m_testUrl);
    //******************************************************************************************************************

    Cookie* cookie3 = new Cookie("TEST3", "VALUE3", "/", expiry);
    driver->manage()->addCookie(cookie3);

    Cookie* cookie31 = driver->manage()->cookieNamed("TEST3");
    compareCookie(cookie3, cookie31, "", m_testUrl);
    //******************************************************************************************************************

    Cookie* cookie4 = new Cookie("TEST4", "VALUE4", "/", m_testUrl, expiry);
    driver->manage()->addCookie(cookie4);

    Cookie* cookie41 = driver->manage()->cookieNamed("TEST4");
    compareCookie(cookie4, cookie41);
    //******************************************************************************************************************

    Cookie* cookie5 = new Cookie("TEST5", "VALUE5", "/", m_testUrl, expiry, true);
    driver->manage()->addCookie(cookie5);

    Cookie* cookie51 = driver->manage()->cookieNamed("TEST5");
    compareCookie(cookie5, cookie51);
    //******************************************************************************************************************

    Cookie* cookie6 = new Cookie("TEST6", "VALUE6", "/", m_testUrl, expiry, true, false);
    driver->manage()->addCookie(cookie6);

    Cookie* cookie61 = driver->manage()->cookieNamed("TEST6");
    compareCookie(cookie6, cookie61);

    int result = checkAllCookies();
    QCOMPARE(result, 9);

    driver->manage()->deleteCookieNamed("TEST1");

    result = checkAllCookies();
    QCOMPARE(result, 8);

    driver->manage()->deleteCookie(cookie2);
    result = checkAllCookies();
    QCOMPARE(result, 7);

    driver->manage()->deleteAllCookies();
    QList<Cookie*> list = driver->manage()->cookies();

    QCOMPARE(list.size(), 0);
}

/******** WINDOW TESTS ************************************************************************************************/

/**
 * Screen FullHD KDE
 */

/**
 * QSize driver->manage()->window()->size()
 */
void QueleniumTest::windowSizeCase()
{
    QSize size = driver->manage()->window()->size();
    QCOMPARE(size.width(), 1440);
    QCOMPARE(size.height(), 782);
}

/**
 * void driver->manage()->window()->setSize(int, int)
 */
void QueleniumTest::windowSetSizeCase1()
{
    driver->manage()->window()->setSize(1350, 698);

    QSize size = driver->manage()->window()->size();

    QCOMPARE(size.width(), 1350);
    QCOMPARE(size.height(), 698);
}

/**
 * void driver->manage()->window()->setSize(QSize)
 */
void QueleniumTest::windowSetSizeCase2()
{
    QSize size(1569, 300);
    driver->manage()->window()->setSize(size);

    QSize size1 = driver->manage()->window()->size();

    QCOMPARE(size, size1);
}

/**
 * void driver->manage()->window()->maximize()
 */
void QueleniumTest::windowMaximizeCase()
{
    QSize size1 = driver->manage()->window()->size();

    driver->manage()->window()->maximize();
    driver->get(m_testUrl);

    QSize size2 = driver->manage()->window()->size();

    QVERIFY(size2.width() > size1.width());
    QVERIFY(size2.height() > size1.height());

    QCOMPARE(size2.width(), 1920);
    QCOMPARE(size2.height(), 1025);
}

/**
 * QPoint driver->manage()->window()->position()
 */
void QueleniumTest::windowPositionCase()
{
    driver->get(m_testUrl);
    QPoint pos = driver->manage()->window()->position();

    QCOMPARE(pos.x(), -2);
    QCOMPARE(pos.y(), 236);
}

/**
 * void driver->manage()->window()->setPosition(int, int)
 */
void QueleniumTest::windowSetPositionCase1()
{
    driver->manage()->window()->setSize(200,200);
    driver->get(m_testUrl);

    driver->manage()->window()->setPosition(500,500);
    driver->navigate()->refresh();

    QPoint pos = driver->manage()->window()->position();

    QCOMPARE(pos.x(), 500);
    QCOMPARE(pos.y(), 500);
}

/**
 * void driver->manage()->window()->setPosition(QPoint)
 */
void QueleniumTest::windowSetPositionCase2()
{
    driver->manage()->window()->setSize(200,200);
    driver->get(m_testUrl);

    QPoint pos(10,24);

    driver->manage()->window()->setPosition(pos);
    driver->navigate()->refresh();

    QPoint pos1 = driver->manage()->window()->position();

    QCOMPARE(pos, pos1);
}

/******** TIMEOUTS TESTS **********************************************************************************************/
/**
 * void driver->manage()->timeouts()->setScriptTimeout(long)
 */
void QueleniumTest::timeoutsSetScriptTimeoutCase()
{
    driver->get(m_testUrl);
    driver->manage()->timeouts()->setScriptTimeout(10000);
}

/**
 * void driver->manage()->timeouts()->pageLoadTimeout(long)
 */
void QueleniumTest::timeoutsPageLoadTimeoutCase()
{
    driver->get(m_testUrl);
    driver->manage()->timeouts()->pageLoadTimeout(10000);
}

/**
 * void driver->manage()->timeouts()->implicitlyWait(long)
 */
void QueleniumTest::timeoutsImplicitlyWaitCase()
{
    driver->get(m_testUrl);
    driver->manage()->timeouts()->implicitlyWait(10000);
}

/******** IMEHANDLER TESTS ********************************************************************************************/
/**
 * QStringList driver->manage()->ime()->availableEngines()
 */
void QueleniumTest::imeGetAvailableEnginesCase()
{
    try {
        /*QStringList list = */driver->manage()->ime()->availableEngines();
    } catch(UnknownErrorException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/**
 * QString driver->manage()->ime()->activeEngine()
 */
void QueleniumTest::imeGetActiveEngineCase()
{
    try {
        /*QString engine = */driver->manage()->ime()->activeEngine();
    } catch(UnknownErrorException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/**
 * void driver->manage()->ime()->activateEngine(QString)
 */
void QueleniumTest::imeActivateEngineCase()
{
    try{
        driver->manage()->ime()->activateEngine("");
    } catch(UnknownErrorException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/**
 * void driver->manage()->ime()->deactivate()
 */
void QueleniumTest::imeDeactivateCase()
{
    try {
        driver->manage()->ime()->deactivate();
    } catch(UnknownErrorException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/**
 * bool driver->manage()->ime()->isActivated()
 */
void QueleniumTest::imeIsActivatedCase()
{
    try {
        /*bool activated = */driver->manage()->ime()->isActivated();
    } catch(UnknownErrorException* e) {
        QFAIL(e->message().toUtf8());
    }
}

/******** LOGS TESTS **************************************************************************************************/
/**
 * QStringList driver->manage()->logs()->availableLogTypes()
 */
void QueleniumTest::logsGetAvailableLogTypesCase()
{
    QStringList list = driver->manage()->logs()->availableLogTypes();

    QCOMPARE(list.size(), 4);
}

/**
 * QList<LogEntry*> driver->manage()->logs()->get(QString);
 */
void QueleniumTest::logsGetCase()
{
    driver->get(m_testUrl);
    driver->manage()->window()->setSize(-1,-1);

    QStringList listTypes = driver->manage()->logs()->availableLogTypes();

    for(int i = 0; i < listTypes.size(); i++)
    {
        QList<LogEntry*> logs = driver->manage()->logs()->get(listTypes.at(i));
        QVERIFY(logs.size() >= 0);
    }
}

/******** MOUSE TESTS *************************************************************************************************/
/**
 * void driver->actions()->mouse()->moveTo(WebElement*)
 */
void QueleniumTest::mouseMoveToCase1()
{
    driver->get(m_testUrl + "/mouse/move_to.html");
    driver->manage()->window()->maximize();

    //TODO change to WebDriver Wait in future
    QThread::sleep(2);

    WebElement* elm1 = driver->findElement(By::id("element1"));

    driver->actions()->mouse()->moveTo(elm1);

    WebElement* elm2 = driver->findElement(By::id("element2"));
    QVERIFY(elm2->isDisplayed());
}

/**
 * void driver->actions()->mouse()->moveTo(int, int)
 */
void QueleniumTest::mouseMoveToCase2()
{
    driver->get(m_testUrl + "/mouse/move_to.html");
    driver->manage()->window()->maximize();

    //TODO change to WebDriver Wait in future
    QThread::sleep(2);

    driver->actions()->mouse()->moveTo(10, 10);

    WebElement* elm = driver->findElement(By::id("element2"));
    QVERIFY(elm->isDisplayed());
}

/**
 * void driver->actions()->mouse()->moveTo(QPoint)
 */
void QueleniumTest::mouseMoveToCase3()
{
    driver->get(m_testUrl + "/mouse/move_to.html");
    driver->manage()->window()->maximize();

    //TODO change to WebDriver Wait in future
    QThread::sleep(2);

    driver->actions()->mouse()->moveTo(QPoint(10, 10));

    WebElement* elm = driver->findElement(By::id("element2"));
    QVERIFY(elm->isDisplayed());
}

/**
 * void driver->actions()->mouse()->click()
 */
void QueleniumTest::mouseClickCase1()
{
    mouseButtonEvents("click - 1", CLICK);
}

/**
 * void driver->actions()->mouse()->click(Mouse::LEFT)
 */
void QueleniumTest::mouseClickCase2()
{
    mouseButtonEvents("click - 1", CLICK, Mouse::LEFT);
}

/**
 * void driver->actions()->mouse()->click(Mouse::MIDDLE)
 */
void QueleniumTest::mouseClickCase3()
{
    mouseButtonEvents("click - 2", CLICK, Mouse::MIDDLE);
}

/**
 * void driver->actions()->mouse()->click(Mouse::RIGHT)
 */
void QueleniumTest::mouseClickCase4()
{
    mouseButtonEvents("contextmenu - 3", CLICK, Mouse::RIGHT);
}

/**
 * void driver->actions()->mouse()->buttondown()
 */
void QueleniumTest::mouseButtonDownCase1()
{
    mouseButtonEvents("mousedown - 1", DOWN);
}

/**
 * void driver->actions()->mouse()->buttondown(Mouse::LEFT)
 */
void QueleniumTest::mouseButtonDownCase2()
{
    mouseButtonEvents("mousedown - 1", DOWN, Mouse::LEFT);
}

/**
 * void driver->actions()->mouse()->buttondown(Mouse::RIGHT)
 */
void QueleniumTest::mouseButtonDownCase3()
{
    //TODO Not work in Selenium Server Standalone 2.x. Only default value (LEFT)
    mouseButtonEvents("mousedown - 3", DOWN, Mouse::RIGHT);
}

/**
 * void driver->actions()->mouse()->buttondown(Mouse::MIDDLE)
 */
void QueleniumTest::mouseButtonDownCase4()
{
    //TODO Not work in Selenium Server Standalone 2.x. Only default value (LEFT)
    mouseButtonEvents("mousedown - 2", DOWN, Mouse::MIDDLE);
}

/**
 * void driver->actions()->mouse()->buttonup()
 */
void QueleniumTest::mouseButtonUpCase1()
{
    mouseButtonEvents("click - 1", UP);
}

/**
 * void driver->actions()->mouse()->buttonup(Mouse::LEFT)
 */
void QueleniumTest::mouseButtonUpCase2()
{
    mouseButtonEvents("click - 1", UP, Mouse::LEFT);
}

/**
 * void driver->actions()->mouse()->buttonup(Mouse::RIGHT)
 */
void QueleniumTest::mouseButtonUpCase3()
{
    mouseButtonEvents("click - 1", UP, Mouse::RIGHT);
}

/**
 * void driver->actions()->mouse()->buttonup(Mouse::MIDDLE)
 */
void QueleniumTest::mouseButtonUpCase4()
{
    mouseButtonEvents("mouseup - 1", UP, Mouse::MIDDLE);
}

/**
 * void driver->actions()->mouse()->doubleclick()
 */
void QueleniumTest::mouseDoubleClickCase1()
{
    mouseButtonEvents("dblclick - 1", CLICK_DOUBLE);
}

/******** WEBDRIVER EXCEPTIONS TESTS **********************************************************************************/
/**
 * UnknownErrorException
 *
 * void driver->get(QString)
 */
void QueleniumTest::getEmptyExceptionCase1()
{
    try {
        driver->get("");
    } catch (UnknownErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not UnknownErrorException");
    }
}

/**
 * UnknownErrorException
 *
 * void driver->get(QUrl)
 */
void QueleniumTest::getEmptyExceptionCase2()
{
    try {
        QUrl url;
        driver->get(url);
    } catch (UnknownErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not UnknownErrorException");
    }
}

/**
 * NoSuchWindowException
 *
 * void driver->get(QString)
 */
void QueleniumTest::getNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        getCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * void driver->get(QUrl)
 */
void QueleniumTest::getNoSuchWindowExceptionCase2()
{
    notFoundWindow();

    try {
        getCase2();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * QString driver->getTitle()
 */
void QueleniumTest::titleNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        titleCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * QString driver->getPageSource()
 */
void QueleniumTest::pageSourceNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        pageSourceCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * QString driver->getWindowHandle()
 */
void QueleniumTest::windowHandleNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        windowHandleCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * QStringList driver->getWindowHandles()
 */
void QueleniumTest::windowHandlesNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        windowHandlesCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::id)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase1()
{
    notFoundWindow();

    try {
        findElementCase1();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::linkText)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase2()
{
    notFoundWindow();

    try {
        findElementCase2();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::partialLinkText)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase3()
{
    notFoundWindow();

    try {
        findElementCase3();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::name)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase4()
{
    notFoundWindow();

    try {
        findElementCase4();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::tagName)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase5()
{
    notFoundWindow();

    try {
        findElementCase5();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::xpath)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase6()
{
    notFoundWindow();

    try {
        findElementCase6();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::className)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase7()
{
    notFoundWindow();

    try {
        findElementCase7();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchWindowException
 *
 * WebElement* driver->findElement(By::cssSelector)
 */
void QueleniumTest::findElementNoSuchWindowExceptionCase8()
{
    notFoundWindow();

    try {
        findElementCase8();
    } catch (NoSuchWindowException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchWindowException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::id)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase1()
{
    try {
        driver->findElement(By::id("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::linkText)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase2()
{
    try {
        driver->findElement(By::linkText("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::partialLinkText)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase3()
{
    try {
        driver->findElement(By::partialLinkText("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::name)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase4()
{
    try {
        driver->findElement(By::name("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::tagName)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase5()
{
    try {
        driver->findElement(By::tagName("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::xpath)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase6()
{
    try {
        driver->findElement(By::xpath("//input[@id='qwertyuiop']"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::className)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase7()
{
    try {
        driver->findElement(By::className("qwertyuiop"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* driver->findElement(By::cssSelector)
 */
void QueleniumTest::findElementNoSuchElementExceptionCase8()
{
    try {
        driver->findElement(By::cssSelector("#qwertyuiop > .ssssssss"));
    } catch(NoSuchElementException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not NoSuchElementException");
    }
}

/**
 * InvalidSelectorException
 *
 * WebElement* driver->findElement(By::xpath)
 */
void QueleniumTest::findElementInvalidSelectorExceptionCase1()
{
    try {
        driver->findElement(By::xpath("//input[ddddd"));
    } catch(InvalidSelectorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not InvalidSelectorException");
    }
}

/**
 * InvalidSelectorException
 *
 * WebElement* driver->findElement(By::cssSelector)
 */
void QueleniumTest::findElementInvalidSelectorExceptionCase2()
{
    try {
        driver->findElement(By::cssSelector("border:bugaga;"));
    } catch(InvalidSelectorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch(...) {
        QFAIL("Not InvalidSelectorException");
    }
}

/******** SELECT TESTS **********************************************************************************/
/**
 * bool element->isMultiple()
 */
void QueleniumTest::selectIsMultiCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    QVERIFY(!elm->isMultiple());
}

/**
 * bool element->isMultiple()
 */
void QueleniumTest::selectIsMultiCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    QVERIFY(elm->isMultiple());
}

/**
 * QList<WebElement*> element->options()
 */
void QueleniumTest::selectOptionsCase1()
{
    driver->get(m_testUrl);

    Select* elm1 = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    Elements opts1 = elm1->options();

    QCOMPARE(opts1.size(), 6);
    /*****************************************************************************************************/

    Select* elm2 = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    Elements opts2 = elm2->options();

    QCOMPARE(opts2.size(), 7);
}

/**
 * QList<WebElement*> element->allSelectedOptions()
 */
void QueleniumTest::selectAllSelectedOptionsCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    Elements opts = elm->allSelectedOptions();

    QCOMPARE(opts.size(), 1);
}

/**
 * QList<WebElement*> element->allSelectedOptions()
 */
void QueleniumTest::selectAllSelectedOptionsCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    Elements opts = elm->allSelectedOptions();

    QCOMPARE(opts.size(), 4);
}

/**
 * WebElement* element->firstSelectedOption()
 */
void QueleniumTest::selectFirstSelectedOptionCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    WebElement* opt = elm->firstSelectedOption();

    QCOMPARE(opt->attribute("value"), QString("item3"));
}

/**
 * WebElement* element->firstSelectedOption()
 */
void QueleniumTest::selectFirstSelectedOptionCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    WebElement* opt = elm->firstSelectedOption();

    QCOMPARE(opt->attribute("value"), QString("item2"));
}

/**
 * void element->selectByVisibleText(QString)
 */
void QueleniumTest::selectByVisibleTextCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    elm->selectByVisibleText("SUBITEM2");

    WebElement* opt = elm->firstSelectedOption();
    QCOMPARE(opt->attribute("value"), QString("subitem2"));
}

/**
 * void element->selectByVisibleText(QString)
 */
void QueleniumTest::selectByVisibleTextCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->selectByVisibleText("SUBITEM1");
    elm->selectByVisibleText("SUBITEM2");

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 6);
}

/**
 * void element->selectByIndex(int)
 */
void QueleniumTest::selectByIndexCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    elm->selectByIndex(5);

    WebElement* opt = elm->firstSelectedOption();
    QCOMPARE(opt->attribute("value"), QString("subitem3"));
}

/**
 * void element->selectByIndex(int)
 */
void QueleniumTest::selectByIndexCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->selectByIndex(0);
    elm->selectByIndex(3);

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 6);
}

/**
 * void element->selectByValue(QString)
 */
void QueleniumTest::selectByValueCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));
    elm->selectByValue("subitem1");

    WebElement* opt = elm->firstSelectedOption();
    QCOMPARE(opt->attribute("value"), QString("subitem1"));
}

/**
 * void element->selectByValue(QString)
 */
void QueleniumTest::selectByValueCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->selectByValue("subitem1");
    elm->selectByValue("subitem3");

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 5);
}

/**
 * void element->deselectAll()
 */
void QueleniumTest::selectDeselectAllCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->deselectAll();

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 0);
}

/**
 * void element->deselectByValue(QString)
 */
void QueleniumTest::selectDeselectByValueCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->deselectByValue("subitem4");
    elm->deselectByValue("item3");

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 2);
}

/**
 * void element->deselectByValue(2)
 */
void QueleniumTest::selectDeselectByIndexCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->deselectByIndex(6);
    elm->deselectByIndex(2);

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 2);
}

/**
 * void element->deselectByVisibleText(QString)
 */
void QueleniumTest::selectDeselectByVisibleTextCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->deselectByVisibleText("SUBITEM3");
    elm->deselectByVisibleText("SUBITEM4");

    Elements opts = elm->allSelectedOptions();
    QCOMPARE(opts.size(), 2);
}

/******** SELECT EXCEPTIONS TESTS **********************************************************************************/
/**
 * WebDriverException
 *
 * Element is not select
 */
void QueleniumTest::selectWebDriverExceptionCase1()
{
    driver->get(m_testUrl);

    try {

        new Select(driver->findElement(By::id("findElementCase1")));

    } catch(WebDriverException* e) {
        QCOMPARE(e->message(), QString("Element is not select"));
    } catch( ... ) {
        QFAIL(QString("Not WebDriverException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * WebElement* element->firstSelectedOption()
 */
void QueleniumTest::selectFirstSelectedOptionNoSuchElementCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));
    elm->deselectAll();

    try {

        elm->firstSelectedOption();

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("No options are selected"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * element->selectByVisibleText(QString)
 */
void QueleniumTest::selectByVisibleTextNoSuchElementCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->selectByVisibleText("BUGAGA");

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with text: BUGAGA"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * void element->selectByVisibleText(QString)
 */
void QueleniumTest::selectByVisibleTextNoSuchElementCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));

    try {

        elm->selectByVisibleText("ITEM1");
        elm->selectByVisibleText("BUGAGA");
        elm->selectByVisibleText("ITEM111");

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with text: BUGAGA"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * void element->selectByIndex(int)
 */
void QueleniumTest::selectByIndexNoSuchElementCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->selectByIndex(100);

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with index: 100"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * void element->selectByIndex(int)
 */
void QueleniumTest::selectByIndexNoSuchElementCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));

    try {

        elm->selectByIndex(3);
        elm->selectByIndex(100);
        elm->selectByIndex(4);

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with index: 100"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * void element->selectByValue(QString)
 */
void QueleniumTest::selectByValueNoSuchElementCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->selectByValue("BUGAGA");

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with value: BUGAGA"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * NoSuchElementException
 *
 * void element->selectByValue(QString)
 */
void QueleniumTest::selectByValueNoSuchElementCase2()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase2")));

    try {

        elm->selectByValue("subitem2");
        elm->selectByValue("BUGAGA");
        elm->selectByValue("subitem1");

    } catch(NoSuchElementException* e) {
        QCOMPARE(e->message(), QString("Cannot locate option with value: BUGAGA"));
    } catch( ... ) {
        QFAIL(QString("Not NoSuchElementException").toUtf8());
    }
}

/**
 * WebDriverException
 *
 * void element->deselectAll()
 */
void QueleniumTest::selectDeselectAllWebDriverExceptionCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->deselectAll();

    } catch(WebDriverException* e) {
        QCOMPARE(e->message(), QString("You may only deselect all options of a multi-select"));
    } catch( ... ) {
        QFAIL(QString("Not WebDriverException").toUtf8());
    }
}

/**
 * WebDriverException
 *
 * void element->deselectByValue(QString)
 */
void QueleniumTest::selectDeselectByValueWebDriverExceptionCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->deselectByValue("item3");

    } catch(WebDriverException* e) {
        QCOMPARE(e->message(), QString("You may only deselect option of a multi-select"));
    } catch( ... ) {
        QFAIL(QString("Not WebDriverException").toUtf8());
    }
}

/**
 * WebDriverException
 *
 * void element->deselectByIndex(int)
 */
void QueleniumTest::selectDeselectByIndexWebDriverExceptionCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->deselectByIndex(2);

    } catch(WebDriverException* e) {
        QCOMPARE(e->message(), QString("You may only deselect option of a multi-select"));
    } catch( ... ) {
        QFAIL(QString("Not WebDriverException").toUtf8());
    }
}

/**
 * WebDriverException
 *
 * void element->deselectByVisibleText(QString)
 */
void QueleniumTest::selectDeselectByVisibleTextWebDriverExceptionCase1()
{
    driver->get(m_testUrl);

    Select* elm = new Select(driver->findElement(By::id("selectIsMultiCase1")));

    try {

        elm->deselectByVisibleText("ITEM3");

    } catch(WebDriverException* e) {
        QCOMPARE(e->message(), QString("You may only deselect option of a multi-select"));
    } catch( ... ) {
        QFAIL(QString("Not WebDriverException").toUtf8());
    }
}

/******** ALERT TESTS **********************************************************************************/
/**
 * void alert->dismiss()
 */
void QueleniumTest::alertDismissCase1()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_confirm"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    alert->dismiss();

    WebElement* result = driver->findElement(By::id("result"));
    QCOMPARE(result->text(), QString("Dismiss"));
}

/**
 * void alert->dismiss()
 */
void QueleniumTest::alertDismissCase2()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_prompt"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    alert->dismiss();

    WebElement* result = driver->findElement(By::id("result"));
    QCOMPARE(result->text(), QString("Dismiss Prompt"));
}

/**
 * void alert->accept()
 */
void QueleniumTest::alertAcceptCase1()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_confirm"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    alert->accept();

    WebElement* result = driver->findElement(By::id("result"));
    QCOMPARE(result->text(), QString("Accept"));
}

/**
 * void alert->accept()
 */
void QueleniumTest::alertAcceptCase2()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_prompt"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    alert->accept();

    WebElement* result = driver->findElement(By::id("result"));
    QCOMPARE(result->text(), QString("Accept Prompt"));
}

/**
 * QString alert->text()
 */
void QueleniumTest::alertTextCase1()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_confirm"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    QString text = alert->text();
    alert->accept();

    QCOMPARE(text, QString("Alert Confirm"));
}

/**
 * QString alert->text()
 */
void QueleniumTest::alertTextCase2()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_prompt"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    QString text = alert->text();
    alert->accept();

    QCOMPARE(text, QString("Alert Prompt"));
}

/**
 * void alert->sendKeys(QString)
 */
void QueleniumTest::alertSendKeysCase()
{
    driver->get(m_testUrl + "/alert/alert.html");

    WebElement* btn = driver->findElement(By::id("alert_prompt_text"));
    btn->click();

    Alert* alert = driver->switchTo()->alert();
    alert->sendKeys("Test Prompt");
    alert->accept();

    WebElement* result = driver->findElement(By::id("result"));
    QCOMPARE(result->text(), QString("Test Prompt"));
}

/******** ALERT EXCEPTIONS TESTS **********************************************************************************/
/**
 * NoAlertOpenErrorException
 *
 * void alert->dismiss()
 */
void QueleniumTest::alertDismissNoAlertPresentCase1()
{
    try {

        driver->get(m_testUrl + "/alert/alert.html");

        Alert* alert = driver->switchTo()->alert();
        alert->dismiss();

    } catch(NoAlertOpenErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch( ... ) {
        QFAIL(QString("Not NoAlertOpenErrorException").toUtf8());
    }
}

/**
 * NoAlertOpenErrorException
 *
 * void alert->accept()
 */
void QueleniumTest::alertAcceptNoAlertPresentCase1()
{
    try {

        driver->get(m_testUrl + "/alert/alert.html");

        Alert* alert = driver->switchTo()->alert();
        alert->accept();

    } catch(NoAlertOpenErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch( ... ) {
        QFAIL(QString("Not NoAlertOpenErrorException").toUtf8());
    }
}

/**
 * NoAlertOpenErrorException
 *
 * QString alert->text()
 */
void QueleniumTest::alertTextNoAlertPresentCase1()
{
    try {

        driver->get(m_testUrl + "/alert/alert.html");

        Alert* alert = driver->switchTo()->alert();
        alert->text();

    } catch(NoAlertOpenErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch( ... ) {
        QFAIL(QString("Not NoAlertOpenErrorException").toUtf8());
    }
}

/**
 * NoAlertOpenErrorException
 *
 * void alert->sendKeys(QString)
 */
void QueleniumTest::alertSendKeysNoAlertPresentCase1()
{
    try {

        driver->get(m_testUrl + "/alert/alert.html");

        Alert* alert = driver->switchTo()->alert();
        alert->sendKeys("");

    } catch(NoAlertOpenErrorException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch( ... ) {
        QFAIL(QString("Not NoAlertOpenErrorException").toUtf8());
    }
}

/**
 * ElementNotVisibleException
 *
 * void alert->sendKeys(QString)
 */
void QueleniumTest::alertSendKeysElementNotVisibleExceptionCase1()
{
    try {

        driver->get(m_testUrl + "/alert/alert.html");

        WebElement* btn = driver->findElement(By::id("alert_confirm"));
        btn->click();

        Alert* alert = driver->switchTo()->alert();
        alert->sendKeys("Test Prompt");


    } catch(ElementNotVisibleException* e) {
        QVERIFY(e->code() != 0);
        QVERIFY(!e->message().isEmpty());
    } catch( ... ) {
        QFAIL(QString("Not ElementNotVisibleException").toUtf8());
    }
}

/******** SERVER TESTS **********************************************************************************/
void QueleniumTest::serverStatusCase()
{
    Status* status = driver->server()->status();

    //OS Linux x64 3.16.0-45-generic
    QCOMPARE(status->os()->arch(), QString("amd64"));
    QCOMPARE(status->os()->name(), QString("Linux"));
    QCOMPARE(status->os()->version(), QString("3.16.0-45-generic"));

    //Selenium Server Standalone
    QCOMPARE(status->build()->version(), QString("2.46.0"));

    //Java
    if(status->java()) {
        QCOMPARE(status->java()->version(), QString("1.8.0_45"));
    }
}

void QueleniumTest::serverSessionsCase()
{
    Proxy *proxy = new Proxy(Proxy::DIRECT);
    DesiredCapabilities *cap = new DesiredCapabilities(Browser::FIREFOX);
    cap->setProxy(proxy);

    int count = 4;
    QList<WebDriver*> list;

    for(int i = 0; i < count; i++) {
        list.push_back(new WebDriver(m_host, m_port, cap, m_testUrl));
    }

    QList<Session*> listSession = driver->server()->sessions();

    for(int i = 0; i < count; i++) {
        list.at(i)->quit();
    }

    QCOMPARE(listSession.size(), 5);

    for(int i = 0; i < listSession.size(); i++) {
        Session* s = listSession.at(i);

        QVERIFY(!s->id().isEmpty());

        DesiredCapabilities* c = s->capabilities();

        QVERIFY(c->isTakesScreenshot());
        QVERIFY(c->isJavascriptEnabled());
        QVERIFY(!c->isRotatable());
        QVERIFY(c->isLocationContextEnabled());
        QCOMPARE(c->browser(), Browser::FIREFOX);
        QCOMPARE(c->platform(), Platform::LINUX);
        QCOMPARE(c->version(), QString("39.0"));
    }

}

QTEST_MAIN(QueleniumTest)
