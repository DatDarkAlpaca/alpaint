#include "pch.h"
#include "Alpaint.h"

Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui.titleBar->installEventFilter(this);

    connectButtons();
}

void Alpaint::mousePressEvent(QMouseEvent* event)
{
    m_LastPoint = event->pos();
}

void Alpaint::mouseMoveEvent(QMouseEvent* event)
{
    if (m_Pressed)
        this->move(mapToParent(event->pos() - m_LastPoint));
}

bool Alpaint::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui.titleBar && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = (QMouseEvent*)event;
        if (!m_Pressed)
            m_LastPoint = mouseEvent->pos();

        m_Pressed = true;
        return true;
    }

    if (object == ui.titleBar && event->type() == QEvent::MouseButtonRelease)
    {
        m_Pressed = false;
        return true;
    }
    else
        return false;
}

void Alpaint::connectButtons()
{
    connect(ui.closeButton, &QPushButton::released, this, &Alpaint::close);
    connect(ui.resizeButton, &QPushButton::released, this, &Alpaint::restoreMaximize);
    connect(ui.minimizeButton, &QPushButton::released, this, &Alpaint::showMinimized);
}

void Alpaint::restoreMaximize()
{
    if (isMaximized())
    {
        ui.resizeButton->setText(QString::fromUtf8("\uE923"));
        restoreGeometry(m_CurrentGeometry);
    }
    else
    {
        ui.resizeButton->setText(QString::fromUtf8("\uE739"));
        m_CurrentGeometry = saveGeometry();
        showMaximized();
    }
}
