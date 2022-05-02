#pragma once
#include "pch.h"
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
			ui.canvas->resize(size);
		}

	private:
		::Ui::CanvasWidget ui;
	};
}