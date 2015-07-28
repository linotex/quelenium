#include "window.h"
#include "webdriverhub.h"

void Window::maximize()
{
    m_hub->windowMaximize(m_windowHandle);
}

QPoint Window::position()
{
    return m_hub->getWindowPosition(m_windowHandle);
}

QSize  Window::size()
{
    return m_hub->getWindowSize(m_windowHandle);
}

void Window::setPosition(QPoint pos)
{
    m_hub->setWindowPosition(m_windowHandle, pos.x(), pos.y());
}

void Window::setPosition(int x, int y)
{
    m_hub->setWindowPosition(m_windowHandle, x, y);
}

void Window::setSize(QSize size)
{
   m_hub->setWindowSize(m_windowHandle, size.width(), size.height());
}

void Window::setSize(int width, int height)
{
    m_hub->setWindowSize(m_windowHandle, width, height);
}
