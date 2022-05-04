#pragma once

namespace alp
{
	constexpr int UndoLimit = 100;
	enum class ToolType { NoType = 0, Pencil, Eraser, Line, Rect, Ellipse, Fill, Count };
}