#ifndef QUELENIUMTEST_H
#define QUELENIUMTEST_H

#include <QObject>
#include <QThread>

#include <WebDriver>
#include <WebDriverException>
#include <Keys>

class QueleniumTest : public QObject
{
    Q_OBJECT
public:
    QueleniumTest();

private:

    enum MOUSE_BUTTON_EVENTS {
        CLICK,
        DOWN,
        UP,
        CLICK_DOUBLE,
    };

    WebDriver* driver;
    typedef QList<WebElement*> Elements;

    int checkAllCookies();
    void compareCookie(Cookie* original, Cookie* tested, QString defaultPath = "", QString defaultDomain = "");
    void notFoundWindow();
    void mouseButtonEvents(QString text, int event, int button = -1);

    QString m_testUrl;

private Q_SLOTS:

    /**
     * WebDriver Test Cases
     */
    void getCase1();
    void getCase2();    
    void titleCase1();
    void pageSourceCase1();
    void windowHandleCase1();
    void windowHandlesCase1();
    void windowHandlesCase2();
    void findElementCase1();
    void findElementCase2();
    void findElementCase3();
    void findElementCase4();
    void findElementCase5();
    void findElementCase6();
    void findElementCase7();
    void findElementCase8();
    void findElementsCase1();
    //void closeCase1();//in future

    /**
     * Navigation Test Cases
     */
    void navigateBackCase();
    void navigateForwardCase();
    void navigateRefreshCase();
    void navigateToCase1();
    void navigateToCase2();

    /**
     * Manage Test Cases
     */
    void manageCookiesCase();
    void manageCookieNamedCase();
    void manageDeleteAllCookiesCase();
    void manageDeleteCookieCase();
    void manageDeleteCookieNamedCase();
    void manageAddCookieCase();

    /**
     * Window Test Cases
     */
    void windowSizeCase();
    void windowSetSizeCase1();
    void windowSetSizeCase2();
    void windowMaximizeCase();
    void windowPositionCase();
    void windowSetPositionCase1();
    void windowSetPositionCase2();
    //void windowCloseCase(); //in future
    //void windowOopenNewTabCase();   //in future
    //void windowOpenNewWindowCase();//in future

    /**
     * Timeouts Test Cases
     */
    void timeoutsSetScriptTimeoutCase();
    void timeoutsPageLoadTimeoutCase();
    void timeoutsImplicitlyWaitCase();

    /**
     * ImeHandler Test Cases
     */
    void imeGetAvailableEnginesCase();
    void imeGetActiveEngineCase();
    void imeActivateEngineCase();
    void imeDeactivateCase();
    void imeIsActivatedCase();

    /**
     * Logs Test Cases
     */
    void logsGetAvailableLogTypesCase();
    void logsGetCase();

    /**
     * Mouse Test Cases
     */
    void mouseMoveToCase1();
    void mouseMoveToCase2();
    void mouseMoveToCase3();
    void mouseClickCase1();
    void mouseClickCase2();
    void mouseClickCase3();
    void mouseClickCase4();
    void mouseButtonDownCase1();
    void mouseButtonDownCase2();
    void mouseButtonDownCase3();
    void mouseButtonDownCase4();
    void mouseButtonUpCase1();
    void mouseButtonUpCase2();
    void mouseButtonUpCase3();
    void mouseButtonUpCase4();
    void mouseDoubleClickCase1();

    /**
     * WebDriver Exceptions Test Cases
     */
    void getEmptyExceptionCase1();
    void getEmptyExceptionCase2();
    void getNoSuchWindowExceptionCase1();
    void getNoSuchWindowExceptionCase2();
    void titleNoSuchWindowExceptionCase1();
    void pageSourceNoSuchWindowExceptionCase1();
    void windowHandleNoSuchWindowExceptionCase1();
    void windowHandlesNoSuchWindowExceptionCase1();
    void findElementNoSuchWindowExceptionCase1();
    void findElementNoSuchWindowExceptionCase2();
    void findElementNoSuchWindowExceptionCase3();
    void findElementNoSuchWindowExceptionCase4();
    void findElementNoSuchWindowExceptionCase5();
    void findElementNoSuchWindowExceptionCase6();
    void findElementNoSuchWindowExceptionCase7();
    void findElementNoSuchWindowExceptionCase8();
    void findElementNoSuchElementExceptionCase1();
    void findElementNoSuchElementExceptionCase2();
    void findElementNoSuchElementExceptionCase3();
    void findElementNoSuchElementExceptionCase4();
    void findElementNoSuchElementExceptionCase5();
    void findElementNoSuchElementExceptionCase6();
    void findElementNoSuchElementExceptionCase7();
    void findElementNoSuchElementExceptionCase8();
    void findElementInvalidSelectorExceptionCase1();
    void findElementInvalidSelectorExceptionCase2();

    /**
     * Select Test Cases
     */
    void selectIsMultiCase1();
    void selectIsMultiCase2();
    void selectOptionsCase1();
    void selectAllSelectedOptionsCase1();
    void selectAllSelectedOptionsCase2();
    void selectFirstSelectedOptionCase1();
    void selectFirstSelectedOptionCase2();
    void selectByVisibleTextCase1();
    void selectByVisibleTextCase2();
    void selectByIndexCase1();
    void selectByIndexCase2();
    void selectByValueCase1();
    void selectByValueCase2();
    void selectDeselectAllCase1();
    void selectDeselectByValueCase1();
    void selectDeselectByIndexCase1();
    void selectDeselectByVisibleTextCase1();

    /**
     * Select Exceptions Test Cases
     */
    void selectWebDriverExceptionCase1();
    void selectFirstSelectedOptionNoSuchElementCase1();
    void selectByVisibleTextNoSuchElementCase1();
    void selectByVisibleTextNoSuchElementCase2();
    void selectByIndexNoSuchElementCase1();
    void selectByIndexNoSuchElementCase2();
    void selectByValueNoSuchElementCase1();
    void selectByValueNoSuchElementCase2();
    void selectDeselectAllWebDriverExceptionCase1();
    void selectDeselectByValueWebDriverExceptionCase1();
    void selectDeselectByIndexWebDriverExceptionCase1();
    void selectDeselectByVisibleTextWebDriverExceptionCase1();

    /**
     * Alert Test Cases
     */
    void alertDismissCase1();
    void alertDismissCase2();
    void alertAcceptCase1();
    void alertAcceptCase2();
    void alertTextCase1();
    void alertTextCase2();
    void alertSendKeysCase();

    /**
     * Alert Exceptions Test Cases
     */
    void alertDismissNoAlertPresentCase1();
    void alertAcceptNoAlertPresentCase1();
    void alertTextNoAlertPresentCase1();
    void alertSendKeysNoAlertPresentCase1();
    void alertSendKeysElementNotVisibleExceptionCase1();

    /**
     * Server Test Cases
     */
    void serverStatusCase();

    void init();
    void cleanup();

};

#endif // QUELENIUMTEST_H
