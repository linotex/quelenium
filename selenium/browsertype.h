#ifndef BROWSERTYPE_H
#define BROWSERTYPE_H

#include <QObject>

class BrowserType
{
public:

    enum BROWSER_TYPE {
        FIREFOX,
        CHROME,
        OPERA,
        IE,
        ANDROID,
        IPHONE,
        IPAD,
        HTMLUNIT,
        SAFARI,
        UNKNOWN
    };

    static QString toString(int type)
    {
        QString name = "";

        switch (type) {

        case FIREFOX:
            name = "firefox";
            break;

        case CHROME:
            name = "chrome";
            break;

        case OPERA:
            name = "opera";
            break;

        case IE:
            name = "internet explorer";
            break;

        case ANDROID:
            name = "android";
            break;

        case IPHONE:
            name = "iphone";
            break;

        case IPAD:
            name = "ipad";
            break;

        case HTMLUNIT:
            name = "htmlunit";
            break;

        case SAFARI:
            name = "safari";
            break;

        default:
            name = "Unknown Name";
            break;
        }

        return name;
    }

    static BROWSER_TYPE fromString(QString text)
    {
        BROWSER_TYPE browser = UNKNOWN;

        text = text.toLower();

        if(text == "firefox") {
            browser = FIREFOX;

        } else if(text == "chrome") {
            browser = CHROME;

        } else if(text == "internet explorer") {
            browser = IE;

        } else if(text == "android") {
            browser = ANDROID;

        } else if(text == "iphone") {
            browser = IPHONE;

        } else if(text == "ipad") {
            browser = IPAD;

        } else if(text == "htmlunit") {
            browser = HTMLUNIT;

        } else if(text == "safari") {
            browser = SAFARI;
        }

        return browser;
    }
};

#endif // BROWSERTYPE_H
