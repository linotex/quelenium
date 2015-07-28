#ifndef IMEHANDLER_H
#define IMEHANDLER_H

#include <QObject>

class WebDriverHub;
class ImeHandler
{
public:
    ImeHandler(WebDriverHub* hub);

    QStringList availableEngines();
    QString activeEngine();
    void activateEngine(QString engine);
    void deactivate();
    bool isActivated();

private:
    WebDriverHub* m_hub;
};

#endif // IMEHANDLER_H
