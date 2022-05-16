#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolUtils.h"

namespace alp
{
	class FillTool : public Tool
	{
	public:
		FillTool() : Tool(ToolType::Fill) { }

	public:
		virtual void draw(QImage& image, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&image);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? ToolHandler::secondaryColor : ToolHandler::primaryColor;
			painter.setPen(QPen(usedColor, ToolHandler::pencilWidth, ToolHandler::pencilStyle, Qt::PenCapStyle::SquareCap));

			QRgb oldColor(image.pixel(endPoint));
	
			if (usedColor.rgb() != oldColor)
				fill(endPoint, oldColor, painter, image);

			int end = 1;
		}
	};
}