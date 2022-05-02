#pragma once
#include "pch.h"
#include "DocumentData.h"
#include "ui_NewFileDialog.h"

namespace alp
{
	class NewFileDialog : public QDialog
	{
		Q_OBJECT

	public:
		NewFileDialog(QWidget* parent)
			: QDialog(parent)
		{
			ui.setupUi(this);
			connectButtons();
		}

	private:
		void connectButtons()
		{
			connect(ui.cancelButton, &QPushButton::clicked, this, &NewFileDialog::reject);
			connect(ui.createButton, &QPushButton::clicked, this, &NewFileDialog::createSlot);
		}

	private Q_SLOTS:
		void createSlot()
		{
			data = DocumentData(ui.widthSpinBox->value(), ui.heightSpinBox->value());
			accept();
		}

	public:
		DocumentData data;

	private:
		Ui::NewFileDialog ui;
	};
}