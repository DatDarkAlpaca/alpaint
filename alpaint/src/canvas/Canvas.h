#pragma once
#include "pch.h"

namespace alp
{
	class Canvas : public QWidget
	{
		Q_OBJECT

	public:
		Canvas(QWidget* parent = nullptr);

	public:
		void resize(const QSize& size);

	protected:
		void mousePressEvent(QMouseEvent* event) override;

		void mouseMoveEvent(QMouseEvent* event) override;

		void mouseReleaseEvent(QMouseEvent* event) override;

		void keyPressEvent(QKeyEvent* event) override;

		void keyReleaseEvent(QKeyEvent* event) override;

		void paintEvent(QPaintEvent* event) override;

		void wheelEvent(QWheelEvent* event) override;

	private:
		void drawLine(const QPoint& endPoint, bool isSecondaryButton);

		void resizeImage(QImage* image, const QSize& newSize);

	private:
		bool m_Drawing = false, m_Panning = false;
		QSize m_Size;
		QPoint m_LastPoint;
		QPixmap m_Pixmap;

	private:
		qreal m_Scale = 1.0;
		QPointF m_Reference;
		QPointF m_Delta;
		QRectF m_Rect;

	private:
		int m_PenWidth = 1;
	};
}