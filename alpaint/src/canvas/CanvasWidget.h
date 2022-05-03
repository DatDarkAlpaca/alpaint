#pragma once
#include "pch.h"
#include "Data.h"
#include "ui_CanvasWidget.h"

namespace alp
{
	class CanvasWidget : public QWidget
	{
		Q_OBJECT

	public:
		CanvasWidget(QWidget* parent, QSize& size)
			: QWidget(parent)
		{
			ui.setupUi(this);
			ui.canvas->resetCanvas(size);
		}

	public:
		Canvas* getCanvas() { return ui.canvas; }

	private:
		::Ui::CanvasWidget ui;
	};
}