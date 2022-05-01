#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Alpaint.h"

class Alpaint : public QMainWindow
{
    Q_OBJECT

public:
    Alpaint(QWidget *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    bool eventFilter(QObject* object, QEvent* event) override;

private:
    void connectButtons();

private Q_SLOTS:
    void restoreMaximize();

private:
    Ui::AlpaintClass ui;

private:
    QByteArray m_CurrentGeometry;
    QPoint m_LastPoint;
    bool m_Pressed = false;
};
