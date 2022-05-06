#pragma once
#include "pch.h"
#include "Constants.h"

namespace alp
{
	class Canvas;

	class Tool : public QPen
	{
	public:
		Tool(ToolType type) : type(type) { }
		virtual ~Tool() = default;

	public:
		Tool& operator=(const Tool&) = delete;
		Tool(const Tool&) = delete;
		
	public:
		virtual void setStartPoint(const QPoint& point) { startPoint = point; }
	
		virtual void setEndPoint(const QPoint& point) { endPoint = point; }

		QPoint getStartPoint() const { return startPoint; }

	public:
		virtual void draw(QPixmap& pixmap, const QPoint& endPoint, bool isSecondaryButton) = 0;

	public:
		ToolType type;

	protected:
		QPoint startPoint, endPoint;
	};
}