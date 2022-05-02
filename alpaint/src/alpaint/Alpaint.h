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

    protected:
        void mousePressEvent(QMouseEvent* event) override;

        void mouseMoveEvent(QMouseEvent* event) override;

    private:
        void connectActions();

    private:
        void newFileAction();

    private:
        ::Ui::AlpaintClass ui;

    private:
        QByteArray m_CurrentGeometry;
        QPoint m_LastPoint;
        bool m_Pressed = false;
    };

}