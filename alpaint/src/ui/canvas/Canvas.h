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

		bool openImage(const QString& filepath)
		{
			QPixmap loadedImage;
			if (!loadedImage.load(filepath))
				return false;

			m_Pixmap = loadedImage;
			resizeCanvas(loadedImage.size());
			//resetCanvasLayers(loadedImage.size());
			//resetCanvasTransform();

			return true;
		}

		bool saveImage(const QByteArray& fileFormat)
		{
			QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
				initialPath,
				tr("%1 Files (*.%2);;All Files (*)")
				.arg(QString::fromLatin1(fileFormat.toUpper()))
				.arg(QString::fromLatin1(fileFormat)));

			if (fileName.isEmpty())
				return false;

			m_Pixmap.save(fileName, fileFormat);
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
		void setCurrentLayerPixmap(const QPixmap& pixmap) { m_Pixmap = pixmap; }

		QPixmap* getSelectedPixmap() { return &m_Pixmap; }

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
		void initializeUndoStack();

		void saveDrawCommand();

		void loadSettings();

	private:
		QPixmap m_Background, m_Pixmap, m_OldPixmap;
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