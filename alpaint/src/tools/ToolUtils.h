#pragma once
#include "pch.h"

namespace alp
{
	inline QPoint getLayerPoint(const QPoint& point, const QRect& canvasRect, const QPointF& delta, qreal scale)
	{
		auto result = (point - canvasRect.center() - delta) / scale;
		result.setX(std::ceil(result.x()) - 1);
		result.setY(std::ceil(result.y()) - 1);

		return result.toPoint();
	}

	inline bool isValid(std::vector<QPoint>&& visited, const QSize& pixmapSize, const QPoint& point)
	{
		for (const auto& visitedPoint : visited)
		{
			if (point == visitedPoint)
				return false;
		}

		if (point.x() < 0 || point.x() >= pixmapSize.width() ||
			point.y() < 0 || point.y() >= pixmapSize.height())
			return false;

		return true;
	}

	inline void fill(const QPoint& point, QRgb oldColor, QPainter& painter, QImage& image)
	{
		std::vector<QPoint> visited;

		std::deque<QPoint> queue;
		queue.push_back(point);

		while (!queue.empty())
		{
			auto currentPos = queue.front();
			queue.pop_front();

			if (!isValid(std::move(visited), image.size(), currentPos))
				continue;

			if (image.pixel(currentPos) == oldColor)
			{
				painter.drawPoint(currentPos);
				visited.push_back(currentPos);

				queue.push_back(QPoint(currentPos.x() + 1, currentPos.y()));
				queue.push_back(QPoint(currentPos.x() - 1, currentPos.y()));
				queue.push_back(QPoint(currentPos.x(), currentPos.y() + 1));
				queue.push_back(QPoint(currentPos.x(), currentPos.y() - 1));
			}
		}
	}
}