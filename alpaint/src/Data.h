#pragma once
#include "pch.h"
#include "tools/Tool.h"

namespace alp
{
	inline QColor primaryColor = QColor(0, 0, 0), secondaryColor = QColor(255, 255, 255);
	inline int pencilWidth = 1;

	inline std::unordered_map<std::string, Tool*> tools;
	inline Tool* currentTool;
}