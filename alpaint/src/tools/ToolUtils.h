#pragma once
#include "pch.h"

namespace alp
{
	inline QPoint getLayerPoint(const QPoint& point, const QImage& image, const QRect& canvasRect, const QPointF& delta, qreal scale)
	{
		auto result = (point - canvasRect.center() + image.scaled(image.size() * scale).rect().center() - delta) / scale;
		result.setX(std::ceil(result.x()) - 1);
		result.setY(std::ceil(result.y()) - 1);

		return result.toPoint();
	}

	inline bool isValid(const QSize& pixmapSize, const QPoint& point)
	{
		if (point.x() < 0 || point.x() >= pixmapSize.width() ||
			point.y() < 0 || point.y() >= pixmapSize.height())
			return false;

		return true;
	}

	inline void fill(const QPoint& point, QRgb oldColor, QPainter& painter, QImage& image)
	{
		std::deque<QPoint> queue;
		queue.push_back(point);

		while (!queue.empty())
		{
			auto currentPos = queue.front();
			queue.pop_front();

			if (!isValid(image.size(), currentPos))
				continue;

			if (image.pixel(currentPos) == oldColor)
			{
				painter.drawPoint(currentPos);

				queue.push_back(QPoint(currentPos.x() + 1, currentPos.y()));
				queue.push_back(QPoint(currentPos.x() - 1, currentPos.y()));				
				queue.push_back(QPoint(currentPos.x(), currentPos.y() + 1));
				queue.push_back(QPoint(currentPos.x(), currentPos.y() - 1));
			}
		}
	}
}