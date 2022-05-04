#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "ToolUtils.h"
#include "canvas/Canvas.h"

namespace alp
{
	class FillTool : public Tool
	{
	public:
		FillTool() : Tool(ToolType::Fill) { }

	public:
		virtual void draw(Canvas* canvas, const QPoint& endPoint, bool isSecondaryButton) override
		{
			if (!canvas)
				return;

			QPainter painter(canvas->getSelectedPixmap());
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, pencilStyle, Qt::PenCapStyle::SquareCap));

			auto fixedEndPoint = getLayerPoint(canvas, endPoint);
			
			QRgb oldColor(canvas->getSelectedPixmap()->toImage().pixel(fixedEndPoint.toPoint()));
	
			if (usedColor.rgb() != oldColor)
				fill(fixedEndPoint.toPoint(), oldColor, painter, canvas);

			canvas->update();
		}
	};
}