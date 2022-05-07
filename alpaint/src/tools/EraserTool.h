#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolHandler.h"

namespace alp
{
	class EraserTool : public Tool
	{
	public:
		EraserTool() : Tool(ToolType::Eraser) { }

	public:
		virtual void draw(QImage& image, const QPoint& endPoint, bool isSecondaryButton) override
		{
			QPainter painter(&image);
			painter.setCompositionMode(QPainter::CompositionMode_Clear);

			painter.eraseRect(QRect(endPoint.x(), endPoint.y(), ToolHandler::pencilWidth, ToolHandler::pencilWidth));
		}
	};
}