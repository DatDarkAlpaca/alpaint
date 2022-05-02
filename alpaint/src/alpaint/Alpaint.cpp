#include "pch.h"
#include "Alpaint.h"
#include "canvas/CanvasWidget.h"
#include "dialogs/NewFileDialog.h"

alp::Alpaint::Alpaint(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connectActions();
}

void alp::Alpaint::connectActions()
{
    connect(ui.actionNew, &QAction::triggered, this, &Alpaint::newFileAction);
}

void alp::Alpaint::newFileAction()
{
    NewFileDialog dialog(this);
    
    if (!dialog.exec())
        return;

    auto data = dialog.data;

    QSize size{ data.documentWidth, data.documentHeight };
    ui.centralWidget->layout()->addWidget(new CanvasWidget(this, size));
}
