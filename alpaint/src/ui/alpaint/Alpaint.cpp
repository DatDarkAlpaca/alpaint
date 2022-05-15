#include "pch.h"
#include "Alpaint.h"
#include "ToolHandler.h"

#include "layers/LayerList.h"
#include "layers/LayerWidget.h"

alp::Alpaint::Alpaint(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    connectTools();
    connectActions();
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

    connect(ui.undoButton, &QToolButton::clicked, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->onUndo();
    });
    connect(ui.redoButton, &QToolButton::clicked, this, [&]() {
        if (m_CurrentProject)
            getFocusedCanvas()->onRedo();
    });

    connect(ToolHandler::tools["picker"].get(), &Tool::colorUpdated, this, [&]() {
        ui.primaryColor->updatePanelColors();
        ui.secondaryColor->updatePanelColors();
    });

    connect(ui.addLayerButton, &QToolButton::clicked, this, [&]() {
        m_CurrentProject->addNewLayer(m_CurrentProject->getCanvas()->getCanvasSize());
    });
    connect(ui.removeLayerButton, &QToolButton::clicked, this, [&]() {
        m_CurrentProject->deleteSelectedLayer();
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
}

void alp::Alpaint::newProjectAction()
{
    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;

    ui.centralWidget->setMaximumSize({ 0, 1000000 });

    // Hide previous projects:
    for (int i = 0; i <= m_ProjectList.count() - 1; ++i)
        m_ProjectList[i]->hideItems();

    // Create a new project:
    m_ProjectList.push_back(new ProjectDock(this, ui.layerList, data.documentSize));
    m_CurrentProject = m_ProjectList.back();
    
    if (m_ProjectList.size() == 1)
    {
        addDockWidget(Qt::RightDockWidgetArea, m_CurrentProject);
        splitDockWidget(ui.toolBar, m_CurrentProject, Qt::Horizontal);
    }
    else
    {
        tabifyDockWidget(m_ProjectList[0], m_CurrentProject);
        m_CurrentProject->show();
        m_CurrentProject->raise();
    }

    // Tab changed:
    auto tab = findChildren<QTabBar*>();
    if (tab.isEmpty())
        return;

    connect(tab.back(), &QTabBar::currentChanged, this, [&](int index) 
    {       
        for (int i = 0; i < m_ProjectList.count(); ++i)
        {
            if (index != i)
                m_ProjectList[i]->hideItems();
        }

        m_ProjectList[index]->showItems();
        m_ProjectList[index]->focusLastLayer();
    });
}

void alp::Alpaint::openProjectAction()
{
    QString initialPath = QDir::currentPath() + "/";

    auto path = QFileDialog::getOpenFileName(this, tr("Open Project"), initialPath,
        tr("ALP Files (*.alp);;All Files (*)"));

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Unable to open the file."), file.errorString());
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_2);

    QString name;
    in >> name;

    size_t size;
    in >> size;

    std::vector<std::shared_ptr<alp::Layer>> result;

    for (size_t i = 0; i < size; ++i)
    {
        QString name;
        in >> name;

        QImage image;
        in >> image;

        QPainter::CompositionMode blendingMode;
        in >> blendingMode;

        auto layer = alp::Layer();
        result.push_back(std::make_shared<alp::Layer>(name, image, blendingMode));
    }

    if (result.empty())
    {
        QMessageBox::warning(this, tr("Invalid project data."), "The file opened is corrupted or is not an Alpaint Project file");
        return;
    }

    ui.centralWidget->setMaximumSize({ 0, 1000000 });

    for (int i = 0; i <= m_ProjectList.count() - 1; ++i)
        m_ProjectList[i]->hideItems();

    m_ProjectList.push_back(new ProjectDock(this, ui.layerList, name, result));
    m_ProjectList.back()->setModified(true);
    m_ProjectList.back()->updateTitle();

    m_CurrentProject = m_ProjectList.back();

    if (m_ProjectList.size() == 1)
    {
        addDockWidget(Qt::RightDockWidgetArea, m_CurrentProject);
        splitDockWidget(ui.toolBar, m_CurrentProject, Qt::Horizontal);
    }
    else
    {
        tabifyDockWidget(m_ProjectList[0], m_CurrentProject);
        m_CurrentProject->show();
        m_CurrentProject->raise();
    }

    // Tab changed:
    auto tab = findChildren<QTabBar*>();
    if (tab.isEmpty())
        return;

    connect(tab.back(), &QTabBar::currentChanged, this, [&](int index) {
        for (int i = 0; i < m_ProjectList.count(); ++i)
        {
            if (index != i)
                m_ProjectList[i]->hideItems();
        }

        m_ProjectList[index]->showItems();
        m_ProjectList[index]->focusLastLayer();
    });
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
        auto message = QMessageBox::warning(this, "Alpaint", "Do you want to save changes to " + m_CurrentProject->getProjectName(), 
            QMessageBox::Save | QMessageBox::Cancel);

        if (message == QMessageBox::Save)
            saveProjectAction();
        
        for (auto it = m_ProjectList.begin(); it < m_ProjectList.end(); )
        {
            auto project = *it;
            if (!project->visibleRegion().isEmpty())
            {
                project->close();
                it = m_ProjectList.erase(it);
            }
            else
                ++it;
        }
    }

    ui.layerList->clear();

    if (m_ProjectList.isEmpty())
        ui.centralWidget->setMaximumSize({ 1000000, 1000000 });
}

void alp::Alpaint::resizeCanvasAction()
{
    if (!m_CurrentProject)
        return;

    //auto image = getFocusedCanvas()->getSelectedLayer();

    //ResizeCanvasDialog dialog(this, image->width(), image->height());

    //if (!dialog.exec())
    //    return;

    //QSize size{ dialog.width, dialog.height };
    //getFocusedCanvas()->resizeCanvas(size);
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
