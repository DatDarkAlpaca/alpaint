#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"

namespace alp
{
	class LineTool : public Tool
	{
	public:
		LineTool() : Tool(ToolType::Line) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint&, bool isSecondaryButton) override
		{
			QPainter painter(&pixmap);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			painter.drawLine(startPoint, this->endPoint);
		}
	};
}