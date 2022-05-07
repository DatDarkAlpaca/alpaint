#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolHandler.h"

namespace alp
{ 
	class PencilTool : public Tool
	{
	public:
		PencilTool() : Tool(ToolType::Pencil) { }

	public:
		virtual void draw(QImage& image, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&image);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? ToolHandler::secondaryColor : ToolHandler::primaryColor;
			painter.setPen(QPen(usedColor, ToolHandler::pencilWidth, ToolHandler::pencilStyle, Qt::PenCapStyle::SquareCap));

			painter.drawPoint(endPoint);
		}
	};
}