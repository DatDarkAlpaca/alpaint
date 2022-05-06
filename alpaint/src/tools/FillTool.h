#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "ToolUtils.h"

namespace alp
{
	class FillTool : public Tool
	{
	public:
		FillTool() : Tool(ToolType::Fill) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&pixmap);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, pencilStyle, Qt::PenCapStyle::SquareCap));

			QRgb oldColor(pixmap.toImage().pixel(endPoint));
	
			if (usedColor.rgb() != oldColor)
				fill(endPoint, oldColor, painter, pixmap);
		}
	};
}