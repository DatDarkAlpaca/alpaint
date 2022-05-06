#pragma once
#include "pch.h"
#include "ui_Alpaint.h"
#include "tools/Tools.h"
#include "canvas/CanvasWidget.h"

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
        void newFileAction();

        void resizeCanvasAction();

    private:
        CanvasWidget* m_CanvasWidget = nullptr;

        ::Ui::AlpaintClass ui;
    };
}