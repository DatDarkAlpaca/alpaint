#pragma once
#include "pch.h"
#include "Data.h"
#include "Tool.h"
#include "canvas/Canvas.h"

namespace alp
{
	class LineTool : public Tool
	{
	public:
		LineTool() : Tool("line") { }

	public:
		virtual void mousePressEvent(Canvas* canvas, QMouseEvent* event) override
		{
			if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
			{
				m_StartPoint = (event->pos() - canvas->rect().center() - canvas->getDelta()) / canvas->getScale();
				m_PixmapCopy = *canvas->getSelectedPixmap();
				m_Drawing = true;
			}
		}

		virtual void mouseMoveEvent(Canvas* canvas, QMouseEvent* event) override
		{
			if (!m_Drawing)
				return;

			canvas->setCurrentLayerPixmap(m_PixmapCopy);

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
		void draw(Canvas* canvas, QPointF endPoint, bool isSecondaryButton)
		{
			if (!canvas)
				return;

			std::cout << m_StartPoint.x() << " " << m_StartPoint.y() << '\n';

			QPainter painter(canvas->getSelectedPixmap());
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

			QColor usedColor = isSecondaryButton ? secondaryColor : primaryColor;
			painter.setPen(QPen(usedColor, pencilWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

			endPoint = (endPoint - canvas->rect().center() - canvas->getDelta()) / canvas->getScale();
			
			painter.drawLine(m_StartPoint, endPoint);

			canvas->update();
		}

	private:
		QPixmap m_PixmapCopy;
		QPointF m_StartPoint;
		bool m_Drawing = false;
	};
}