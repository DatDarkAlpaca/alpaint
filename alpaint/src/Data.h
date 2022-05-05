#pragma once
#include "pch.h"
#include "tools/Tool.h"

namespace alp
{
	inline QColor primaryColor = QColor(0, 0, 0), secondaryColor = QColor(255, 255, 255);
	inline Qt::PenStyle pencilStyle = Qt::SolidLine;
	inline int pencilWidth = 1;

	inline std::unordered_map<std::string, Tool*> tools;
	inline Tool* currentTool;

	inline void setTool(const std::string& name)
	{
		if (tools.find(name) == tools.end())
			return;

		currentTool = tools[name];
	}

	inline bool enableGrid = true, enableSanityBackground = true;
}