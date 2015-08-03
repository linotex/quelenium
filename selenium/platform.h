#ifndef PLATFORM_H
#define PLATFORM_H

#include <QObject>

class Platform
{
public:
    enum Type {
        WINDOWS,
        XP,
        VISTA,
        MAC,
        LINUX,
        UNIX,
        ANDROID,
        UNKNOWN
    };

    static QString toString(Platform::Type platform)
    {
        QString name = "";

        switch (platform) {

        case WINDOWS:
            name = "WINDOWS";
            break;

        case XP:
            name = "XP";
            break;

        case VISTA:
            name = "VISTA";
            break;

        case MAC:
            name = "MAC";
            break;

        case LINUX:
            name = "LINUX";
            break;

        case UNIX:
            name = "UNIX";
            break;

        case ANDROID:
            name = "ANDROID";
            break;

        default:
            name = "Unknown Platform";
            break;
        }

        return name;
    }

    Platform::Type static fromString(QString platform)
    {
        Platform::Type id = UNKNOWN;

        if(platform == "WINDOWS")      { id = WINDOWS; }
        else if(platform == "XP")      { id = XP; }
        else if(platform == "VISTA")   { id = VISTA; }
        else if(platform == "MAC")     { id = MAC; }
        else if(platform == "LINUX")   { id = LINUX; }
        else if(platform == "UNIX")    { id = UNIX; }
        else if(platform == "ANDROID") { id = ANDROID; }

        return id;
    }
};

#endif // PLATFORM_H
