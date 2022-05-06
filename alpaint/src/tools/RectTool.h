#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"

namespace alp
{
	class RectTool : public Tool
	{
	public:
		RectTool() : Tool(ToolType::Rect) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&pixmap);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			if (startPoint != endPoint)
				painter.drawRect(QRectF(startPoint, endPoint));
			else
				painter.drawPoint(startPoint);
		}
	};
}