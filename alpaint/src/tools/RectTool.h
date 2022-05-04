#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "ToolUtils.h"
#include "canvas/Canvas.h"

namespace alp
{
	class RectTool : public Tool
	{
	public:
		RectTool() : Tool(ToolType::Rect) { }

	public:
		virtual void draw(Canvas* canvas, const QPoint& endPoint, bool isSecondaryButton) override
		{
			if (!canvas)
				return;

			QPainter painter(canvas->getSelectedPixmap());
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			auto fixedStartPoint = getLayerPoint(canvas, startPoint);
			auto fixedEndPoint = getLayerPoint(canvas, endPoint);

			if (startPoint != endPoint)
				painter.drawRect(QRectF(fixedStartPoint, fixedEndPoint));
			else
				painter.drawPoint(fixedStartPoint);

			canvas->update();
		}
	};
}