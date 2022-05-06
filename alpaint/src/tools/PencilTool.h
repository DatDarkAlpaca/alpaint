#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"

namespace alp
{ 
	class PencilTool : public Tool
	{
	public:
		PencilTool() : Tool(ToolType::Pencil) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&pixmap);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, pencilStyle, Qt::PenCapStyle::SquareCap));

			painter.drawPoint(endPoint);
		}
	};
}