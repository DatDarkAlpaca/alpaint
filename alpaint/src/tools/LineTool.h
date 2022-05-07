#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolHandler.h"

namespace alp
{
	class LineTool : public Tool
	{
	public:
		LineTool() : Tool(ToolType::Line) { }

	public:
		virtual void draw(QImage& image, const QPoint&, bool isSecondaryButton) override
		{
			QPainter painter(&image);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? ToolHandler::secondaryColor : ToolHandler::primaryColor;
			painter.setPen(QPen(usedColor, ToolHandler::pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			painter.drawLine(startPoint, this->endPoint);
		}
	};
}