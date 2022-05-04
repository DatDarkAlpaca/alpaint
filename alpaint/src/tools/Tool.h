#pragma once
#include "pch.h"

namespace alp
{
	class Canvas;

	class Tool
	{
	public:
		Tool(const std::string& name) : name(name) { }

	public:
		virtual void mousePressEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseMoveEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseReleaseEvent(Canvas* canvas, QMouseEvent* event) = 0;

	public:
		std::string name;
	};
}