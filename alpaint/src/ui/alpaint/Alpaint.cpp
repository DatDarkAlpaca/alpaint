#include "pch.h"
#include "Alpaint.h"
#include "ToolHandler.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    connectActions();
    connectTools();
}

void alp::Alpaint::keyPressEvent(QKeyEvent* event)
{
    if(m_CanvasWidget)
        ToolHandler::keyPressEvent(event, m_CanvasWidget->getCanvas());
}

void alp::Alpaint::keyReleaseEvent(QKeyEvent* event)
{
    if (m_CanvasWidget)
        ToolHandler::keyReleaseEvent(event, m_CanvasWidget->getCanvas());
}

void alp::Alpaint::connectTools()
{
    connect(ui.pencilButton,      &QToolButton::clicked, this, [&]() { ToolHandler::setTool("pencil");  });
    connect(ui.eraserButton,      &QToolButton::clicked, this, [&]() { ToolHandler::setTool("eraser");  });
    connect(ui.rectButton,        &QToolButton::clicked, this, [&]() { ToolHandler::setTool("rect");    });
    connect(ui.ellipseButton,     &QToolButton::clicked, this, [&]() { ToolHandler::setTool("ellipse"); });
    connect(ui.colorPickerButton, &QToolButton::clicked, this, [&]() { ToolHandler::setTool("picker");  });
    connect(ui.fillButton,        &QToolButton::clicked, this, [&]() { ToolHandler::setTool("fill");    });

    connect(ToolHandler::tools["picker"].get(), &Tool::colorUpdated, this, [&]() {
        ui.primaryColor->updatePanelColors();
        ui.secondaryColor->updatePanelColors();
    });
}

void alp::Alpaint::connectActions()
{
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newFileAction);
    connect(ui.actionResizeCanvas, &QAction::triggered, this, &Alpaint::resizeCanvasAction);

    connect(ui.actionShowPixelGrid, &QAction::triggered, this, [&]() {
        if (m_CanvasWidget)
            m_CanvasWidget->getCanvas()->toggleGrid();
    });
    connect(ui.actionShowBackground, &QAction::triggered, this, [&]() {
        if (m_CanvasWidget)
            m_CanvasWidget->getCanvas()->toggleBackground();
    });

    connect(ui.actionUndo, &QAction::triggered, this, [&]() {
        if (m_CanvasWidget)
        {
            auto canvas = m_CanvasWidget->getCanvas();
            canvas->onUndo();
        }
    });

    connect(ui.actionRedo, &QAction::triggered, this, [&]() {
        if (m_CanvasWidget)
        {
            auto canvas = m_CanvasWidget->getCanvas();
            canvas->onRedo();
        }
    });
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

void alp::Alpaint::resizeCanvasAction()
{
    if (!m_CanvasWidget)
        return;

    auto canvas = m_CanvasWidget->getCanvas();
    auto pixmap = canvas->getSelectedPixmap();

    ResizeCanvasDialog dialog(this, pixmap->width(), pixmap->height());

    if (!dialog.exec())
        return;

    QSize size{ dialog.width, dialog.height };
    canvas->resizeCanvas(size);
}
