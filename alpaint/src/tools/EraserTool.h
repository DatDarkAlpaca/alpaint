#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "ToolUtils.h"
#include "canvas/Canvas.h"

namespace alp
{
	class EraserTool : public Tool
	{
	public:
		EraserTool() : Tool(ToolType::Eraser) { }

	public:
		virtual void draw(Canvas* canvas, const QPoint& endPoint, bool isSecondaryButton) override
		{
			if (!canvas)
				return;

			QPainter painter(canvas->getSelectedPixmap());
			painter.setCompositionMode(QPainter::CompositionMode_Clear);

			auto point = getLayerPoint(canvas, endPoint);
			painter.eraseRect(QRect(point.x(), point.y(), pencilWidth, pencilWidth));

			canvas->update();
		}
	};
}