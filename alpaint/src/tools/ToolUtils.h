#pragma once
#include "canvas/Canvas.h"

namespace alp
{
	inline QPointF getLayerPoint(Canvas* canvas, QPoint point)
	{
		auto result = (point - canvas->rect().center() - canvas->getDelta()) / canvas->getScale();
		point.setX(std::ceil(point.x()) - 1);
		point.setY(std::ceil(point.y()) - 1);

		return result;
	}
}