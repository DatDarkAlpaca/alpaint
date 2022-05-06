#pragma once
#include "pch.h"
#include "ToolHandler.h"

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

			QColor color;
			if (objectName() == "primaryColor")
			{
				color = QColorDialog::getColor(ToolHandler::primaryColor, nullptr, "Select Color");
				ToolHandler::primaryColor = color;
			}
			else if (objectName() == "primaryColor")
			{
				color = QColorDialog::getColor(ToolHandler::secondaryColor, nullptr, "Select Color");
				ToolHandler::secondaryColor = color;
			}

			updatePanelColors();
		}

	public:
		void updatePanelColors()
		{
			auto color = objectName() == "primaryColor" ? ToolHandler::primaryColor : ToolHandler::secondaryColor;
			QString style = "background-color: rgb(%1, %2, %3);";
			setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
		}
	};
}
