#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>

class Browser
{
public:

    enum Type {
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

    static Type fromString(QString text)
    {
        Type browser = UNKNOWN;

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

#endif // BROWSER_H
