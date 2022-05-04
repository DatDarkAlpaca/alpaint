#pragma once
#include "pch.h"
#include "tools/Tool.h"

namespace alp
{
	class Canvas : public QWidget
	{
		Q_OBJECT

	public:
		Canvas(QWidget* parent = nullptr);

	public:
		void resetCanvasLayers(const QSize& size);

		void resetCanvasTransform();

	public:
		void setCurrentLayerPixmap(const QPixmap& pixmap) { m_Pixmap = pixmap; }

		QPixmap* getSelectedPixmap() { return &m_Pixmap; }

		void togglePixelGrid(bool enable) { m_EnableGrid = enable; }

		bool enabledPixelGrid() { return m_EnableGrid; }

		void toggleSanityBackground(bool enable) { m_EnableSanityBackground = enable; }

		bool enabledSanityBackground() { return m_EnableSanityBackground; }

		qreal getScale() const { return m_Scale; }

		QPointF getDelta() const { return m_Delta; }

	public:
		void mousePressEvent(QMouseEvent* event) override;

		void mouseMoveEvent(QMouseEvent* event) override;

		void mouseReleaseEvent(QMouseEvent* event) override;

		void keyPressEvent(QKeyEvent* event) override;

		void keyReleaseEvent(QKeyEvent* event) override;

		void paintEvent(QPaintEvent* event) override;

		void wheelEvent(QWheelEvent* event) override;

	private:
		QPixmap m_Background, m_Pixmap;
		QSize m_Size;

	private:
		QPointF m_Reference, m_Delta;
		qreal m_Scale = 1.0;
		QRectF m_Rect;

		bool m_Panning = false;

	private:
		bool m_EnableGrid = true, m_EnableSanityBackground = true;
	};
}