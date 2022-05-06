#include "pch.h"
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
    if (event->key() == Qt::Key_Q && m_CanvasWidget)
        m_CanvasWidget->getCanvas()->resetCanvasTransform();

    if (event->key() == Qt::Key_Shift)
    {
        if (currentTool->type == ToolType::Pencil)
            currentTool = tools["line"];
    }
    else if (event->key() == Qt::Key_Alt)
    {
        if (currentTool->type == ToolType::Pencil)
            currentTool = tools["picker"];
    }

    if (m_CanvasWidget)
        m_CanvasWidget->getCanvas()->keyPressEvent(event);
}

void alp::Alpaint::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Shift)
    {
        if (currentTool->type == ToolType::Line)
            currentTool = tools["pencil"];
    }
    else if (event->key() == Qt::Key_Alt)
    {
        if (currentTool->type == ToolType::ColorPicker)
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
    tools["ellipse"] = new EllipseTool();
    tools["fill"] = new FillTool();
    tools["picker"] = new ColorPickerTool();

    currentTool = tools["pencil"];
}

void alp::Alpaint::connectTools()
{
    connect(ui.pencilButton,  &QToolButton::clicked, this, [&]() { setTool("pencil");  });
    connect(ui.eraserButton,  &QToolButton::clicked, this, [&]() { setTool("picker");  });
    connect(ui.rectButton,    &QToolButton::clicked, this, [&]() { setTool("rect");    });
    connect(ui.ellipseButton, &QToolButton::clicked, this, [&]() { setTool("ellipse"); });

    connect(tools["picker"], &Tool::colorUpdated, this, [&]() {
        ui.primaryColor->updatePanelColors();
        ui.secondaryColor->updatePanelColors();
    });
}

void alp::Alpaint::connectActions()
{
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newFileAction);
    connect(ui.actionShowPixelGrid, &QAction::triggered, this, [&]() {
        enableGrid = !enableGrid;
    });
    connect(ui.actionShowBackground, &QAction::triggered, this, [&]() {
        enableSanityBackground = !enableSanityBackground;
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
