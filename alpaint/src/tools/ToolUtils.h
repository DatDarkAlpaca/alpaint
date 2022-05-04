#pragma once
#include "canvas/Canvas.h"

namespace alp
{
	inline QPointF getLayerPoint(Canvas* canvas, QPoint point)
	{
		auto result = (point - canvas->rect().center() - canvas->getDelta()) / canvas->getScale();
		result.setX(std::ceil(result.x()) - 1);
		result.setY(std::ceil(result.y()) - 1);

		return result;
	}
}