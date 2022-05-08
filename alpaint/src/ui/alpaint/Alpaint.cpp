﻿#include "pch.h"
#include "Alpaint.h"
#include "ToolHandler.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setDockNestingEnabled(true);
    setFocus();

    connectActions();
    connectTools();
}

void alp::Alpaint::keyPressEvent(QKeyEvent* event)
{
    if(m_CurrentProject)
        ToolHandler::keyPressEvent(event, getFocusedCanvas());
}

void alp::Alpaint::keyReleaseEvent(QKeyEvent* event)
{
    if (m_CurrentProject)
        ToolHandler::keyReleaseEvent(event, getFocusedCanvas());
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
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newProjectAction);
    connect(ui.actionOpen, &QAction::triggered, this, &Alpaint::openProjectAction);
    connect(ui.actionSave, &QAction::triggered, this, &Alpaint::saveProjectAction);
    connect(ui.actionSaveAs, &QAction::triggered, this, &Alpaint::saveAsProjectAction);
    connect(ui.actionClose, &QAction::triggered, this, &Alpaint::closeProjectAction);

    connect(ui.actionResizeCanvas, &QAction::triggered, this, &Alpaint::resizeCanvasAction);

    connect(ui.actionShowPixelGrid, &QAction::triggered, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->toggleGrid();
        });
    connect(ui.actionShowBackground, &QAction::triggered, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->toggleBackground();
        });

    connect(ui.actionUndo, &QAction::triggered, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->onUndo();
        });

    connect(ui.actionRedo, &QAction::triggered, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->onRedo();
    });
}

void alp::Alpaint::newProjectAction()
{
    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;

    m_ProjectList.push_back(new ProjectDock(this, new Canvas(this, data.documentSize)));
    auto project = m_ProjectList.back();
    m_CurrentProject = project;

    connect(project->getCanvas(), &Canvas::projectModified, project, [project]() {
        project->setModified(true);
        project->updateTitle();
    });

    if (m_ProjectList.size() == 1)
        addDockWidget(Qt::RightDockWidgetArea, project);
    else
    {
        tabifyDockWidget(m_ProjectList[0], project);
        project->show();
        project->raise();
    }
}

void alp::Alpaint::openProjectAction()
{
    /*auto filepath = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath());
    
    if (filepath.length() <= 0)
        return;

    if (!m_Canvas)
    {
        m_Canvas = new Canvas(this);
        ui.centralWidget->layout()->addWidget(m_Canvas);
    }
    
    m_Canvas->openImage(filepath);*/
}

void alp::Alpaint::saveProjectAction()
{
    if (!m_CurrentProject)
        return;

    if (m_CurrentProject->isDefault())
        m_CurrentProject->saveNewProject();
    else
        m_CurrentProject->saveChanges();
}

void alp::Alpaint::saveAsProjectAction()
{
    if (!m_CurrentProject)
        return;

    m_CurrentProject->saveNewProject();
}

void alp::Alpaint::closeProjectAction()
{
    if(!m_CurrentProject->modified())
        m_CurrentProject->close();
    else
    {
        // Todo: dialog for 'Do you want to save changes to ProjectName?'
        // Save || Discard || Cancel
        m_CurrentProject->close();
    }
}

void alp::Alpaint::resizeCanvasAction()
{
    if (!m_CurrentProject)
        return;

    auto image = getFocusedCanvas()->getSelectedLayer();

    ResizeCanvasDialog dialog(this, image->width(), image->height());

    if (!dialog.exec())
        return;

    QSize size{ dialog.width, dialog.height };
    getFocusedCanvas()->resizeCanvas(size);
}

alp::Canvas* alp::Alpaint::getFocusedCanvas()
{
    for (auto dockProject : m_ProjectList)
    {
        if (!dockProject->visibleRegion().isEmpty())
            return dockProject->getCanvas();
    }

    return nullptr;
}
