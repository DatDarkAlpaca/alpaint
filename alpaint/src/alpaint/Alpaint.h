#pragma once
#include "pch.h"
#include "ui_Alpaint.h"
#include "tools/Tool.h"
#include "canvas/CanvasWidget.h"

namespace alp
{
    class Alpaint : public QMainWindow
    {
        Q_OBJECT

    public:
        Alpaint(QWidget* parent = Q_NULLPTR);

    private:
        void initializeTools();

    private:
        void connectActions();

    private:
        void newFileAction();

    private:
        CanvasWidget* m_CanvasWidget = nullptr;
        ::Ui::AlpaintClass ui;
    };
}