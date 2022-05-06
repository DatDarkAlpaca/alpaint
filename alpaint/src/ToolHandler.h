#pragma once
#include "pch.h"
#include "tools/Tool.h"

namespace alp
{
	class ToolHandler
	{
	public:
		ToolHandler& operator=(const ToolHandler&) = delete;
		ToolHandler(const ToolHandler&) = delete;
		ToolHandler() = delete;

	public:
		static void initializeTools();

	public:
		static void setTool(std::string&& name) { currentTool = tools[name]; }

	public:
		static inline QColor primaryColor = Qt::black, secondaryColor = Qt::white;
		static inline Qt::PenStyle pencilStyle = Qt::SolidLine;
		static inline int pencilWidth = 1;

	public:
		static inline std::unordered_map<std::string, std::shared_ptr<Tool>> tools;
		static inline std::shared_ptr<Tool> currentTool;
	};
}