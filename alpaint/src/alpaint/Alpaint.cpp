﻿#include "pch.h"
#include "Alpaint.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    connectActions();

    initializeTools();

    connectTools();    
}

void alp::Alpaint::keyPressEvent(QKeyEvent* event)
{
    if (m_CanvasWidget)
        m_CanvasWidget->getCanvas()->keyPressEvent(event);

    if (event->key() == Qt::Key_Shift)
    {
        if (currentTool->name == "pencil")
            currentTool = tools["line"];
    }
}

void alp::Alpaint::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Shift)
    {
        if(currentTool->name == "line")
            currentTool = tools["pencil"];
    }

    if (m_CanvasWidget)
        m_CanvasWidget->getCanvas()->keyReleaseEvent(event);    
}

void alp::Alpaint::initializeTools()
{
    tools["pencil"] = new PencilTool();
    tools["eraser"] = new EraserTool();
    tools["line"] = new LineTool();
    tools["rect"] = new RectTool();

    currentTool = tools["pencil"];
}

void alp::Alpaint::connectTools()
{
    connect(ui.pencilButton, &QToolButton::clicked, this, [&]() { setTool("pencil"); });
    connect(ui.eraserButton, &QToolButton::clicked, this, [&]() { setTool("eraser"); });
    connect(ui.rectButton,   &QToolButton::clicked, this, [&]() { setTool("rect");   });
}

void alp::Alpaint::connectActions()
{
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newFileAction);
}

void alp::Alpaint::newFileAction()
{
    if (m_CanvasWidget)
        return;

    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;

    QSize size{ data.documentWidth, data.documentHeight };
    m_CanvasWidget = new CanvasWidget(this, size);

    ui.centralWidget->layout()->addWidget(m_CanvasWidget);
}
