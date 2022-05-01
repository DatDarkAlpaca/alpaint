#include "pch.h"
#include "Alpaint.h"

Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
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