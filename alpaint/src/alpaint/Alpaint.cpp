﻿#include "pch.h"
#include "Data.h"
#include "Alpaint.h"

#include "tools/PencilTool.h"
#include "tools/EraserTool.h"

#include "dialogs/NewFileDialog.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connectActions();

    initializeTools();

    connectTools();
}

void alp::Alpaint::initializeTools()
{
    tools["pencil"] = new PencilTool();
    tools["eraser"] = new EraserTool();

    currentTool = tools["pencil"];
}

void alp::Alpaint::connectTools()
{
    connect(ui.pencilButton, &QToolButton::clicked, this, [&]() { setTool("pencil"); });
    connect(ui.eraserButton, &QToolButton::clicked, this, [&]() { setTool("eraser"); });
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
