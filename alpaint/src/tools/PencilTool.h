#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "ToolUtils.h"
#include "canvas/Canvas.h"

namespace alp
{ 
	class PencilTool : public Tool
	{
	public:
		PencilTool() : Tool(ToolType::Pencil) { }

	public:
		virtual void draw(Canvas* canvas, const QPoint& endPoint, bool isSecondaryButton) override
		{
			if (!canvas)
				return;

			QPainter painter(canvas->getSelectedPixmap());
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, pencilStyle, Qt::PenCapStyle::SquareCap));

			auto point = getLayerPoint(canvas, endPoint);
			painter.drawPoint(point);

			canvas->update();
		}
	};
}