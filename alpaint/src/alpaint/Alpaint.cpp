#include "pch.h"
#include "Alpaint.h"
#include "dialogs/NewFileDialog.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connectActions();
}

void alp::Alpaint::mousePressEvent(QMouseEvent* event)
{
    m_LastPoint = event->pos();
}

void alp::Alpaint::mouseMoveEvent(QMouseEvent* event)
{
    if (m_Pressed)
        this->move(mapToParent(event->pos() - m_LastPoint));
}

void alp::Alpaint::connectActions()
{
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newFileAction);
}

void alp::Alpaint::newFileAction()
{
    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;
}
