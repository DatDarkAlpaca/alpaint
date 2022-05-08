#pragma once
#include "pch.h"
#include "tools/Tool.h"

namespace alp
{
	class Canvas : public QWidget
	{
		Q_OBJECT

	public:
		Canvas(QWidget* parent = nullptr, QSize size = QSize());

	public:
		void resetCanvasLayers(const QSize& size);

		void resetCanvasTransform();

		void resizeCanvas(const QSize& size);

		void openImage(const QString& filepath)
		{
			QImageReader reader(filepath);

			m_CurrentLayer = reader.read();
			m_Size = m_CurrentLayer.size();
			m_Scale = int(std::min(m_CurrentLayer.size().width(), m_CurrentLayer.size().height()) / 4);;
			m_Delta = QPointF(0, 0);
			update();
		}

		void saveNewImage(const QString& projectName, const QByteArray& fileFormat)
		{
			/*QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
				initialPath,
				tr("%1 Files (*.%2);;All Files (*)")
				.arg(QString::fromLatin1(fileFormat.toUpper()))
				.arg(QString::fromLatin1(fileFormat)));

			if (fileName.isEmpty())
				return;

			m_CurrentLayer.save(fileName, fileFormat);*/
		}

	public:
		void toggleGrid()
		{
			QSettings settings;
			m_EnableGrid = !m_EnableGrid;
			settings.beginGroup("canvas");
			settings.setValue("enableGrid", m_EnableGrid);
			settings.endGroup();
		}

		void toggleBackground()
		{
			QSettings settings;
			m_EnableSanityBackground = !m_EnableSanityBackground;
			settings.beginGroup("canvas");
			settings.setValue("enableBackground", m_EnableSanityBackground);
			settings.endGroup();
		}

	public slots:
		void onUndo();

		void onRedo();

	public:
		void setCurrentLayer(const QImage& pixmap) { m_CurrentLayer = pixmap; }

		QImage* getSelectedLayer() { return &m_CurrentLayer; }

		qreal getScale() const { return m_Scale; }

		QPointF getDelta() const { return m_Delta; }

		// Preparations the layer blending modes in the future:
		QImage getPreparedImage() const { return m_CurrentLayer; }

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
		QImage m_Background, m_CurrentLayer, m_OldLayer;
		bool m_Drawing = false, m_DrawingLine = false;
		QUndoStack* m_UndoStack;
		QSize m_Size;

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