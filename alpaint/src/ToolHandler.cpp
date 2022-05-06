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
