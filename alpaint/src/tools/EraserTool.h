#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"

namespace alp
{
	class EraserTool : public Tool
	{
	public:
		EraserTool() : Tool(ToolType::Eraser) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&pixmap);
			painter.setCompositionMode(QPainter::CompositionMode_Clear);

			painter.eraseRect(QRect(endPoint.x(), endPoint.y(), pencilWidth, pencilWidth));
		}
	};
}