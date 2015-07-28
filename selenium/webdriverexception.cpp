#include "webdriverexception.h"

void WebDriverException::throwException(QString message, int code)
{
    switch (code) {

    case ResponseStatusCodes::NO_SUCH_DRIVER:
        throw (new NoSuchDriverException(message, code));
        break;

    case ResponseStatusCodes::NO_SUCH_ELEMENT:
        throw (new NoSuchElementException(message, code));
        break;

    case ResponseStatusCodes::NO_SUCH_FRAME:
        throw (new NoSuchFrameException(message, code));
        break;

    case ResponseStatusCodes::UNKNOWN_COMMAND:
        throw (new UnknownCommandException(message, code));
        break;

    case ResponseStatusCodes::STALE_ELEMENT_REFERENCE:
        throw (new StaleElementReferenceException(message, code));
        break;

    case ResponseStatusCodes::ELEMENT_NOT_VISIBLE:
        throw (new ElementNotVisibleException(message, code));
        break;

    case ResponseStatusCodes::INVALID_ELEMENT_STATE:
        throw (new InvalidElementStateException(message, code));
        break;

    case ResponseStatusCodes::UNKNOWN_ERROR:
        throw (new UnknownErrorException(message, code));
        break;

    case ResponseStatusCodes::ELEMENT_IS_NOT_SELECTABLE:
        throw (new ElementIsNotSelectableException(message, code));
        break;

    case ResponseStatusCodes::JAVA_SCRIPT_ERROR:
        throw (new JavaScriptErrorException(message, code));
        break;

    case ResponseStatusCodes::XPATH_LOOKUP_ERROR:
        throw (new XpathLookupErrorException(message, code));
        break;

    case ResponseStatusCodes::TIMEOUT:
        throw (new TimeoutException(message, code));
        break;

    case ResponseStatusCodes::NO_SUCH_WINDOW:
        throw (new NoSuchWindowException(message, code));
        break;

    case ResponseStatusCodes::INVALID_COOKIE_DOMAIN:
        throw (new InvalidCookieDomainException(message, code));
        break;

    case ResponseStatusCodes::UNABLE_TO_SET_COOKIE:
        throw (new UnableToSetCookieException(message, code));
        break;

    case ResponseStatusCodes::UNEXPECTED_ALERT_OPEN:
        throw (new UnexpectedAlertOpenException(message, code));
        break;

    case ResponseStatusCodes::NO_ALERT_OPEN_ERROR:
        throw (new NoAlertOpenErrorException(message, code));
        break;

    case ResponseStatusCodes::SCRIPT_TIMEOUT:
        throw (new ScriptTimeoutException(message, code));
        break;

    case ResponseStatusCodes::INVALID_ELEMENT_COORDINATES:
        throw (new InvalidElementCoordinatesException(message, code));
        break;

    case ResponseStatusCodes::IME_NOT_AVAILABLE:
        throw (new ImeNotAvailableException(message, code));
        break;

    case ResponseStatusCodes::IME_ENGINE_ACTIVATION_FAILED:
        throw (new ImeEngineActivationFailedException(message, code));
        break;

    case ResponseStatusCodes::INVALID_SELECTOR:
        throw (new InvalidSelectorException(message, code));
        break;

    case ResponseStatusCodes::SESSION_NOT_CREATED_EXCEPTION:
        throw (new SessionNotCreatedException(message, code));
        break;

    case ResponseStatusCodes::MOVE_TARGET_OUT_OF_BOUNDS:
        throw (new MoveTargetOutOfBoundsException(message, code));
        break;

    default:
        throw (new WebDriverException(message, code));
        break;
    }
}
