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

	public:
		void updatePanelColors()
		{
			auto color = objectName() == "primaryColor" ? primaryColor : secondaryColor;
			QString style = "background-color: rgb(%1, %2, %3);";
			setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
		}

	protected:
		void mousePressEvent(QMouseEvent* event) override
		{
			if (event->button() != Qt::LeftButton)
				return;

			QColor color;
			if (objectName() == "primaryColor")
			{
				color = QColorDialog::getColor(primaryColor, nullptr, "Select Color");
				primaryColor = color;
			}
			else if (objectName() == "primaryColor")
			{
				color = QColorDialog::getColor(secondaryColor, nullptr, "Select Color");
				secondaryColor = color;
			}

			updatePanelColors();

			if(tools["pencil"])
				currentTool = tools["pencil"];
		}
	};
}
