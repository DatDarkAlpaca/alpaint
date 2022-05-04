#pragma once
#include "pch.h"
#include "Tool.h"
#include "Data.h"
#include "canvas/Canvas.h"

namespace alp
{
	class EraserTool : public Tool
	{
	public:
		EraserTool() : Tool("eraser") { }

	public:
		virtual void mousePressEvent(Canvas* canvas, QMouseEvent* event) override
		{
			if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
			{
				draw(canvas, event->pos(), event->button() == Qt::RightButton);
				m_Drawing = true;
			}
		}

		virtual void mouseMoveEvent(Canvas* canvas, QMouseEvent* event) override
		{
			if (!m_Drawing)
				return;

			if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
				draw(canvas, event->pos(), event->buttons() & Qt::RightButton);
		}

		virtual void mouseReleaseEvent(Canvas* canvas, QMouseEvent* event) override
		{
			if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
			{
				draw(canvas, event->pos(), event->button() == Qt::RightButton);
				m_Drawing = false;
			}
		}

	private:
		void draw(Canvas* canvas, QPoint endPoint, bool isSecondaryButton)
		{
			if (!canvas)
				return;

			QPainter painter(canvas->getSelectedPixmap());
			painter.setCompositionMode(QPainter::CompositionMode_Clear);

			auto point = (endPoint - canvas->rect().center() - canvas->getDelta()) / canvas->getScale();
			painter.eraseRect(QRect(point.x(), point.y(), pencilWidth, pencilWidth));

			canvas->update();
		}

	private:
		bool m_Drawing = false;
	};
}