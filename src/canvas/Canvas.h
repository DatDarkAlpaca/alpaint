#pragma once
#include "pch.h"
#include <QtWidgets/qwidget.h>

class Canvas : public QFrame
{
	Q_OBJECT

public:
	Canvas(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

private:
	void drawLine(const QPoint& endPoint);

	void resizeImage(QImage* image, const QSize& newSize);

private:
	QImage m_Image;
	QPoint m_LastPoint {0, 0};

	bool m_Drawing = false;

private:
	QColor m_PenColor = QColor(210, 210, 210);
	int m_PenWidth = 1;
};