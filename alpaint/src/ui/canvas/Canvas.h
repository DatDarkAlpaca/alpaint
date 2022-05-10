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
		Canvas(QWidget* parent = nullptr, QSize size = QSize());

	public:
		void selectLayer(const std::shared_ptr<Layer>& layer)
		{
			m_CurrentLayer = layer;
		}

		void deleteCurrentLayer()
		{
			delete m_CurrentLayer.get();
			m_CurrentLayer = nullptr;
		}

	public:
		void resetCanvasTransform();

		void resizeCanvas(const QSize& size);

		void openImage(const QString& filepath)
		{
			/*QImageReader reader(filepath);

			m_CurrentLayer = reader.read();
			m_Size = m_CurrentLayer.size();
			m_Scale = int(std::min(m_CurrentLayer.size().width(), m_CurrentLayer.size().height()) / 4);;
			m_Delta = QPointF(0, 0);
			update();*/
		}

	public:
		void toggleGrid();

		void toggleBackground();

	public slots:
		void onUndo();

		void onRedo();

	public:
		qreal getScale() const { return m_Scale; }

		QPointF getDelta() const { return m_Delta; }

		// Preparations the layer blending modes in the future:
		// QImage getPreparedImage() const { return m_CurrentLayer; }

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

	private:
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