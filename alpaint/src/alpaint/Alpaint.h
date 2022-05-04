#pragma once
#include "pch.h"
#include "Data.h"
#include "ui_Alpaint.h"
#include "tools/Tools.h"
#include "canvas/CanvasWidget.h"
#include "dialogs/NewFileDialog.h"

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
        void initializeTools();

        void connectTools();

    private:
        void connectActions();

    private:
        void newFileAction();

    private:
        CanvasWidget* m_CanvasWidget = nullptr;

        ::Ui::AlpaintClass ui;
    };
}