#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"

namespace alp
{
	class ColorPickerTool : public Tool
	{
	public:
		ColorPickerTool() : Tool(ToolType::ColorPicker) { }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) override
		{
			auto color = pixmap.toImage().pixelColor(endPoint);

			if (!isSecondaryButton)
				primaryColor = color;
			else
				secondaryColor = color;

			emit colorUpdated();
		}
	};
}