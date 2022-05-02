#pragma once
#include "ui_Alpaint.h"
#include "pch.h"

namespace alp
{
    class Alpaint : public QMainWindow
    {
        Q_OBJECT

    public:
        Alpaint(QWidget* parent = Q_NULLPTR);

    private:
        void connectActions();

    private:
        void newFileAction();

    private:
        ::Ui::AlpaintClass ui;
    };
}