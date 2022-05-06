#pragma once
#include "pch.h"
#include "DocumentData.h"
#include "ui_ResizeCanvasDialog.h"

namespace alp
{
	class ResizeCanvasDialog : public QDialog
	{
		Q_OBJECT

	public:
		ResizeCanvasDialog(QWidget* parent, int currentWidth, int currentHeight)
			: QDialog(parent), width(currentWidth), height(currentHeight)
		{
			ui.setupUi(this);
			connectButtons();

			ui.widthSpinBox->setValue(width);
			ui.heightSpinBox->setValue(height);
		}

	private:
		void connectButtons()
		{
			connect(ui.cancelButton, &QPushButton::clicked, this, &ResizeCanvasDialog::reject);
			connect(ui.resizeButton, &QPushButton::clicked, this, &ResizeCanvasDialog::createSlot);
		}

	private Q_SLOTS:
		void createSlot()
		{
			width = ui.widthSpinBox->value();
			height = ui.heightSpinBox->value();

			if (width > 0 && width <= 5000 && height > 0 && height <= 5000)
				accept();
		}

	public:
		int width, height;

	private:
		::Ui::ResizeDialog ui;
	};
}