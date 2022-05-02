#pragma once
#include "pch.h"
#include "Data.h"

namespace alp
{
	class ColorPicker : public QFrame
	{
		Q_OBJECT

	public:
		ColorPicker(QWidget* parent)
			: QFrame(parent) { }

	protected:
		void mousePressEvent(QMouseEvent* event) override
		{
			if (event->button() != Qt::LeftButton)
				return;

			QColor color = QColorDialog::getColor(primaryColor, nullptr, "Select Color");

			// Todo: Try something less hacky:
			if (this->objectName() == "primaryColor")
				alp::primaryColor = color;
			else if (this->objectName() == "secondaryColor")
				alp::secondaryColor = color;

			QString style = "background-color: rgb(%1, %2, %3);";
			this->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
		}
	};
}
