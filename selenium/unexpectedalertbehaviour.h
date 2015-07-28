#ifndef UNEXPECTEDALERTBEHAVIOUR_H
#define UNEXPECTEDALERTBEHAVIOUR_H

#include <QObject>

class UnexpectedAlertBehaviour
{
public:

    enum UNEXPECTED_ALERT_BEHAVIOUR {
        ACCEPT,
        DISMISS,
        IGNORE
    };

    static QString toString(int type)
    {
        QString name = "";

        switch (type) {

        case ACCEPT:
            name = "accept";
            break;

        case DISMISS:
            name = "dismiss";
            break;

        case IGNORE:
            name = "ignore";
            break;

        default:
            name = "Unknown Type";
            break;
        }

        return name;
    }
};

#endif // UNEXPECTEDALERTBEHAVIOUR_H
