#pragma once
#include "pch.h"
#include "tools/Tool.h"
#include "layers/Layer.h"
#include "layers/LayerWidget.h"

namespace alp
{
	class Canvas : public QWidget
	{
		Q_OBJECT

	public:
		Canvas(QWidget* parent, QSize size, std::vector<std::shared_ptr<Layer>>& layers);

	public:
		void selectLayer(std::shared_ptr<Layer>& layer) { m_CurrentLayer = layer; }

		std::shared_ptr<Layer>& getLayer() { return m_CurrentLayer; }

	public:
		void resetCanvasTransform();

		void resizeCanvas(const QSize& size);

	public:
		void toggleGrid();

		void toggleBackground();

	public slots:
		void onUndo();

		void onRedo();

	public:
		qreal getScale() const { return m_Scale; }

		QPointF getDelta() const { return m_Delta; }

		QSize getCanvasSize() const { return m_Size; }

	public:
		void mousePressEvent(QMouseEvent* event) override;

		void mouseMoveEvent(QMouseEvent* event) override;

		void mouseReleaseEvent(QMouseEvent* event) override;

		void keyPressEvent(QKeyEvent* event) override;

		void keyReleaseEvent(QKeyEvent* event) override;

		void paintEvent(QPaintEvent* event) override;

		void wheelEvent(QWheelEvent* event) override;

	signals:
		void projectModified();

	private:
		void initializeUndoStack();

		void saveDrawCommand();

		void loadSettings();

	private:
		QImage m_Background, m_OldLayer;
		bool m_Drawing = false, m_DrawingLine = false;
		QUndoStack* m_UndoStack;
		QSize m_Size;

	private:
		std::vector<std::shared_ptr<Layer>>& m_LayersRef;
		std::shared_ptr<Layer> m_CurrentLayer = nullptr;

	private:
		QPointF m_Reference, m_Delta;
		qreal m_Scale = 1.0;
		QRectF m_Rect;

		bool m_Panning = false;

	private:
		bool m_EnableGrid, m_EnableSanityBackground;
		int m_UndoLimit;
	};
}