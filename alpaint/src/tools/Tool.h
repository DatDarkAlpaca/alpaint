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

	inline void adjustPainterPosition(const QPixmap& pixmap, qreal scale, QRect canvasRect, QPointF delta, QPainter* painter)
	{
		painter->translate(-delta / scale);
		painter->translate((-canvasRect.width() / 2) / scale + (pixmap.rect().width() / 2) / scale,
			(-canvasRect.height() / 2) / scale + (pixmap.rect().height() / 2) / scale);
	}

	class Tool
	{
	public:
		virtual void mousePressEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseMoveEvent(Canvas* canvas, QMouseEvent* event) = 0;

		virtual void mouseReleaseEvent(Canvas* canvas, QMouseEvent* event) = 0;
	};
}