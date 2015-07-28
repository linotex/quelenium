#ifndef RESPONSESTATUSCODES_H
#define RESPONSESTATUSCODES_H

class ResponseStatusCodes
{
public:
    enum {
        SUCCESS                         = 0,
        NO_SUCH_DRIVER                  = 6,
        NO_SUCH_ELEMENT                 = 7,
        NO_SUCH_FRAME                   = 8,
        UNKNOWN_COMMAND                 = 9,
        STALE_ELEMENT_REFERENCE         = 10,
        ELEMENT_NOT_VISIBLE             = 11,
        INVALID_ELEMENT_STATE           = 12,
        UNKNOWN_ERROR                   = 13,
        ELEMENT_IS_NOT_SELECTABLE       = 15,
        JAVA_SCRIPT_ERROR               = 17,
        XPATH_LOOKUP_ERROR              = 19,
        TIMEOUT                         = 21,
        NO_SUCH_WINDOW                  = 23,
        INVALID_COOKIE_DOMAIN           = 24,
        UNABLE_TO_SET_COOKIE            = 25,
        UNEXPECTED_ALERT_OPEN           = 26,
        NO_ALERT_OPEN_ERROR             = 27,
        SCRIPT_TIMEOUT                  = 28,
        INVALID_ELEMENT_COORDINATES     = 29,
        IME_NOT_AVAILABLE               = 30,
        IME_ENGINE_ACTIVATION_FAILED    = 31,
        INVALID_SELECTOR                = 32,
        SESSION_NOT_CREATED_EXCEPTION   = 33,
        MOVE_TARGET_OUT_OF_BOUNDS       = 34,
    };
};

#endif // RESPONSESTATUSCODES_H
