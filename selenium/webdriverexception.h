#ifndef WEBDRIVEREXCEPTION_H
#define WEBDRIVEREXCEPTION_H

#include <QObject>

#include "queleniumexception.h"

class WebDriverException : public QueleniumException
{
public:
    explicit WebDriverException(QString message, int code = 0):
        QueleniumException(message, code)
    {}
    ~WebDriverException() throw() {}

    static void throwException(QString message, int code = 0);

    void raise() const { throw *this; }
    WebDriverException *clone() const { return new WebDriverException(*this); }

};

class NoSuchDriverException : public WebDriverException
{
public:
    explicit NoSuchDriverException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~NoSuchDriverException() throw() {}
};

class NoSuchElementException : public WebDriverException
{
public:
    explicit NoSuchElementException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~NoSuchElementException() throw() {}
};

class NoSuchFrameException : public WebDriverException
{
public:
    explicit NoSuchFrameException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~NoSuchFrameException() throw() {}
};

class UnknownCommandException : public WebDriverException
{
public:
    explicit UnknownCommandException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~UnknownCommandException() throw() {}
};

class StaleElementReferenceException : public WebDriverException
{
public:
    explicit StaleElementReferenceException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~StaleElementReferenceException() throw() {}
};

class ElementNotVisibleException : public WebDriverException
{
public:
    explicit ElementNotVisibleException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~ElementNotVisibleException() throw() {}
};

class InvalidElementStateException : public WebDriverException
{
public:
    explicit InvalidElementStateException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~InvalidElementStateException() throw() {}
};

class UnknownErrorException : public WebDriverException
{
public:
    explicit UnknownErrorException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~UnknownErrorException() throw() {}
};

class ElementIsNotSelectableException : public WebDriverException
{
public:
    explicit ElementIsNotSelectableException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~ElementIsNotSelectableException() throw() {}
};

class JavaScriptErrorException : public WebDriverException
{
public:
    explicit JavaScriptErrorException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~JavaScriptErrorException() throw() {}
};

class XpathLookupErrorException : public WebDriverException
{
public:
    explicit XpathLookupErrorException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~XpathLookupErrorException() throw() {}
};

class TimeoutException : public WebDriverException
{
public:
    explicit TimeoutException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~TimeoutException() throw() {}
};

class NoSuchWindowException : public WebDriverException
{
public:
    explicit NoSuchWindowException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~NoSuchWindowException() throw() {}
};

class InvalidCookieDomainException : public WebDriverException
{
public:
    explicit InvalidCookieDomainException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~InvalidCookieDomainException() throw() {}
};

class UnableToSetCookieException : public WebDriverException
{
public:
    explicit UnableToSetCookieException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~UnableToSetCookieException() throw() {}
};

class UnexpectedAlertOpenException : public WebDriverException
{
public:
    explicit UnexpectedAlertOpenException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~UnexpectedAlertOpenException() throw() {}
};

class NoAlertOpenErrorException : public WebDriverException
{
public:
    explicit NoAlertOpenErrorException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~NoAlertOpenErrorException() throw() {}
};

class ScriptTimeoutException : public WebDriverException
{
public:
    explicit ScriptTimeoutException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~ScriptTimeoutException() throw() {}
};

class InvalidElementCoordinatesException : public WebDriverException
{
public:
    explicit InvalidElementCoordinatesException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~InvalidElementCoordinatesException() throw() {}
};

class ImeNotAvailableException : public WebDriverException
{
public:
    explicit ImeNotAvailableException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~ImeNotAvailableException() throw() {}
};

class ImeEngineActivationFailedException : public WebDriverException
{
public:
    explicit ImeEngineActivationFailedException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~ImeEngineActivationFailedException() throw() {}
};

class InvalidSelectorException : public WebDriverException
{
public:
    explicit InvalidSelectorException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~InvalidSelectorException() throw() {}
};

class SessionNotCreatedException : public WebDriverException
{
public:
    explicit SessionNotCreatedException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~SessionNotCreatedException() throw() {}
};

class MoveTargetOutOfBoundsException : public WebDriverException
{
public:
    explicit MoveTargetOutOfBoundsException(QString message, int code = 0):
        WebDriverException(message, code)
    {}
    ~MoveTargetOutOfBoundsException() throw() {}
};

#endif // WEBDRIVEREXCEPTION_H
