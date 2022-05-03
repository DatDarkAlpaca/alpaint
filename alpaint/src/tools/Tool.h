#pragma once
#include "pch.h"

namespace alp
{
	class Canvas;

	inline QPoint getRelativePosition(QMouseEvent* event, qreal scale)
	{
		if (!event)
			return QPoint(0, 0);

		auto x = std::floor(event->pos().x() / scale);
		auto y = std::floor(event->pos().y() / scale);

		return QPoint(x, y);
	}

	class Tool
	{
	public:
		virtual void mousePressEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseMoveEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseReleaseEvent(Canvas* canvas, QMouseEvent* event) = 0;
	};
}