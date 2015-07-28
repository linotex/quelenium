#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QSize>
#include <QPoint>

class WebDriverHub;
class Window
{
public:
    explicit Window(QString windowHandle, WebDriverHub* hub) : m_windowHandle(windowHandle), m_hub(hub) {}

    void maximize();
    QPoint position();
    QSize  size();

    void setPosition(QPoint pos);
    void setPosition(int x, int y);

    void setSize(QSize size);
    void setSize(int width, int height);

    //void close(); //in future
    //void openNewTab(bool switchTo = false);   //in future
    //void openNewWindow(bool switchTo = false);//in future

private:
    QString m_windowHandle;
    WebDriverHub* m_hub;
};

#endif // WINDOW_H
