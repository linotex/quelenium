#ifndef WEBDRIVERHUB_H
#define WEBDRIVERHUB_H

#include <QSize>
#include <QPoint>
#include <QHash>
#include <QList>

#include "seleniumserverhub.h"
#include "desiredcapabilities.h"
#include "proxy.h"
#include "browsertype.h"
#include "unexpectedalertbehaviour.h"
#include "platform.h"

class WebDriverHub : public SeleniumServerHub
{

public:
    explicit WebDriverHub(QString host, int port, DesiredCapabilities *dc);
    ~WebDriverHub();

    DesiredCapabilities* capabilities(bool refresh);

    QJsonObject status();
    QJsonArray  sessions();

    void startSession();
    void deleteSession();

    void setTimeouts(QString type, int ms);
    void setTimeoutsAsyncScript(int ms);
    void setTimeoutsImplicitWait(int ms);

    QString getWindowHandle();
    QStringList getWindowHandles();

    void setUrl(QString url);
    QString getUrl();

    void forward();
    void back();
    void refresh();

    QJsonObject executeJS(QString script, QStringList args);
    QJsonObject executeAsyncJS(QString script, QStringList args);

    QString getScreenshot();

    QStringList getImeAvailableEngines();
    QString     getImeActiveEngine();
    bool        isImeActivated();
    void        imeDeactivate();
    void        imeActivate(QString engine);

    void changeFrame(QString id);
    void changeFrame(int id);
    void changeFrame(QJsonObject id);
    void changeFrame();
    void changeFrameParent();

    void   changeWindow(QString name);
    void   deleteWindow();
    void   setWindowSize(QString handle, int width, int height);
    QSize  getWindowSize(QString handle);
    void   setWindowPosition(QString handle, int x, int y);
    QPoint getWindowPosition(QString handle);
    void   windowMaximize(QString handle);

    QList<QJsonObject> getAllCookie();
    void setCookie(QJsonObject cookie);
    void deleteAllCookie();
    void deleteCookie(QString name);

    QString getSource();
    QString getTitle();

    QString     getElement(QString by, QString value);
    QStringList getElements(QString by, QString value);
    QString     getElementActive();
    void        elementId(QString id);//This command is reserved for future use; its return type is currently undefined.
    QString     getElementByElement(QString id, QString by, QString value);
    QStringList getElementsByElement(QString id, QString by, QString value);
    void        elementClick(QString id);
    void        elementSubmit(QString id);
    QString     getElementText(QString id);
    void        elementValue(QString id, QStringList values);
    void        keys(QString text);
    QString     getElementTagName(QString id);
    void        elementClear(QString id);
    bool        isElementSelected(QString id);
    bool        isElementEnabled(QString id);
    QString     getElementAttributeValue(QString id, QString name);
    bool        elementEqualsOther(QString id, QString other);
    bool        isElementDisplayed(QString id);
    QPoint      getElementLocation(QString id);
    QPoint      getElementLocationInView(QString id);
    QSize       getElementSize(QString id);
    QString     getElementCssPropertyValue(QString id, QString propertyName);

    QString  getOrientation();
    void     setOrientation(QString orientation);

    QString getAlertText();
    void    setAlertText(QString text);
    void    acceptAlert();
    void    dismissAlert();

    void mouseMoveTo(QString element);
    void mouseMoveTo(int xoffset, int yoffset);
    void mouseClick(int button);
    void mouseButtonDown(int button);
    void mouseButtonUp(int button);
    void mouseDoubleClick();

    void touchClick(QString element);
    void touchDown(int x, int y);
    void touchUp(int x, int y);
    void touchMove(int x, int y);
    void touchScroll(QString element, int xoffset, int yoffset);
    void touchScroll(int xoffset, int yoffset);
    void touchDoubleClick(QString element);
    void touchLongClick(QString element);
    void touchFlick(QString element, int xoffset, int yoffset, int speed);
    void touchFlick(int xspeed, int yspeed);

    QJsonObject getGeoLocation();
    void        setGeoLocation(int latitude, int longtude, int alttude);

    QStringList getAllKeysLocalStorage();
    void        setLocalStorageByKey(QString key, QString value);
    void        deleteLocalStorage();
    QString     getLocalStorageValueByKey(QString key);
    void        deleteLocalStorageByKey(QString key);
    int         getLocalStorageSize();

    QStringList getAllKeysSessionStorage();
    void        setSessionStorageByKey(QString key, QString value);
    void        deleteSessionStorage();
    QString     getSessionStorageValueByKey(QString key);
    void        deleteSessionStorageKey(QString key);
    int         getSessionStorageSize();

    QList<QJsonObject> getLog(QString type);
    QStringList        getLogTypes();

    int getApplicationCacheStatus();

protected:

    void updateCapability(QJsonObject json);

    DesiredCapabilities* m_dc;

};

#endif // WEBDRIVERHUB_H
