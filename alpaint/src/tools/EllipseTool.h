#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolHandler.h"

namespace alp
{
	class EllipseTool : public Tool
	{
	public:
		EllipseTool() : Tool(ToolType::Ellipse) { }

	public:
		virtual void draw(QImage& image, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&image);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? ToolHandler::secondaryColor : ToolHandler::primaryColor;
			painter.setPen(QPen(usedColor, ToolHandler::pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			if (startPoint != endPoint)
				painter.drawEllipse(QRectF(startPoint, endPoint));
			else
				painter.drawPoint(startPoint);
		}
	};
}