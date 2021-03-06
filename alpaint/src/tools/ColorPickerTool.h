#pragma once
#include "pch.h"
#include "Tool.h"
#include "ToolHandler.h"

namespace alp
{
	class ColorPickerTool : public Tool
	{
	public:
		ColorPickerTool() : Tool(ToolType::ColorPicker) { }

	public:
		virtual void draw(QImage& image, const QPoint& endPoint, bool isSecondaryButton) override
		{
			auto color = image.pixelColor(endPoint);

			if (!isSecondaryButton)
				ToolHandler::primaryColor = color;
			else
				ToolHandler::secondaryColor = color;

			if (ToolHandler::tools["picker"])
				ToolHandler::setTool("pencil");

			emit colorUpdated();
		}
	};
}