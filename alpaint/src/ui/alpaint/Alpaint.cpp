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
    if(m_Canvas)
        ToolHandler::keyPressEvent(event, m_Canvas);
}

void alp::Alpaint::keyReleaseEvent(QKeyEvent* event)
{
    if (m_Canvas)
        ToolHandler::keyReleaseEvent(event, m_Canvas);
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
    connect(ui.actionOpen, &QAction::triggered, this, &Alpaint::openFileAction);
    connect(ui.actionSave, &QAction::triggered, this, &Alpaint::saveFileAction);

    connect(ui.actionResizeCanvas, &QAction::triggered, this, &Alpaint::resizeCanvasAction);

    connect(ui.actionShowPixelGrid, &QAction::triggered, this, [&]() {
        if (m_Canvas)
            m_Canvas->toggleGrid();
    });
    connect(ui.actionShowBackground, &QAction::triggered, this, [&]() {
        if (m_Canvas)
            m_Canvas->toggleBackground();
    });

    connect(ui.actionUndo, &QAction::triggered, this, [&]() {
        if (m_Canvas)
            m_Canvas->onUndo();
    });

    connect(ui.actionRedo, &QAction::triggered, this, [&]() {
        if (m_Canvas)
            m_Canvas->onRedo();
    });
}

void alp::Alpaint::newFileAction()
{
    if (m_Canvas)
        return;

    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;

    QSize size{ data.documentWidth, data.documentHeight };
    m_Canvas = new Canvas(this, size);

    ui.centralWidget->layout()->addWidget(m_Canvas);
}

void alp::Alpaint::openFileAction()
{
    auto filepath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath());
    
    if (filepath.length() <= 0)
        return;

    if (!m_Canvas)
    {
        m_Canvas = new Canvas(this);
        ui.centralWidget->layout()->addWidget(m_Canvas);
    }
    
    m_Canvas->openImage(filepath);
}

void alp::Alpaint::saveFileAction()
{
    if (!m_Canvas)
        return;

    m_Canvas->saveImage("png");
}

void alp::Alpaint::resizeCanvasAction()
{
    if (!m_Canvas)
        return;

    auto pixmap = m_Canvas->getSelectedPixmap();

    ResizeCanvasDialog dialog(this, pixmap->width(), pixmap->height());

    if (!dialog.exec())
        return;

    QSize size{ dialog.width, dialog.height };
    m_Canvas->resizeCanvas(size);
}
