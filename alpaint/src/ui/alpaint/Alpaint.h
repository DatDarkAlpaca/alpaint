#pragma once
#include "pch.h"
#include "ui_Alpaint.h"
#include "tools/Tools.h"

#include "docks/ProjectDock.h"
#include "dialogs/NewFileDialog.h"
#include "dialogs/ResizeCanvasDialog.h"

namespace alp
{
    class Alpaint : public QMainWindow
    {
        Q_OBJECT

    public:
        Alpaint(QWidget* parent = Q_NULLPTR);

    protected:
        void keyPressEvent(QKeyEvent* event) override;

        void keyReleaseEvent(QKeyEvent* event) override;
        
    private:
        void connectTools();

    private:
        void connectActions();

    private:
        void newProjectAction();

        void openProjectAction();

        void saveProjectAction();

        void saveAsProjectAction();

        void closeProjectAction();

    private:
        void resizeCanvasAction();

    private:
        Canvas* getFocusedCanvas();

    private:
        QList<ProjectDock*> m_ProjectList;
        ProjectDock* m_CurrentProject = nullptr;

        ::Ui::AlpaintClass ui;
    };
}