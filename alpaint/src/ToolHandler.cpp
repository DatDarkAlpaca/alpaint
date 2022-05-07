#include "pch.h"
#include "ToolHandler.h"
#include "tools/Tools.h"

void alp::ToolHandler::initializeTools()
{
    tools["pencil"] = std::make_shared<PencilTool>();
    tools["eraser"] = std::make_shared<EraserTool>();
    tools["line"] = std::make_shared<LineTool>();
    tools["rect"] = std::make_shared<RectTool>();
    tools["ellipse"] = std::make_shared<EllipseTool>();
    tools["fill"] = std::make_shared<FillTool>();
    tools["picker"] = std::make_shared<ColorPickerTool>();

    currentTool = tools["pencil"];
}

void alp::ToolHandler::keyPressEvent(QKeyEvent* event, Canvas* canvas)
{
	if (event->key() == Qt::Key_Q && canvas)
		canvas->resetCanvasTransform();

	if (event->key() == Qt::Key_Shift)
	{
		if (currentTool->type == ToolType::Pencil)
			setTool("line");
	}
	else if (event->key() == Qt::Key_Alt)
	{
		if (currentTool->type == ToolType::Pencil)
			setTool("picker");
	}

	if (canvas)
		canvas->keyPressEvent(event);
}

void alp::ToolHandler::keyReleaseEvent(QKeyEvent* event, Canvas* canvas)
{
	if (event->key() == Qt::Key_Shift)
	{
		if (currentTool->type == ToolType::Line)
			setTool("pencil");
	}
	else if (event->key() == Qt::Key_Alt)
	{
		if (currentTool->type == ToolType::ColorPicker)
			setTool("pencil");
	}

	if (canvas)
		canvas->keyReleaseEvent(event);
}
