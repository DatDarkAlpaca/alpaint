#include "pch.h"
#include "Canvas.h"
#include <iostream>

Canvas::Canvas(QWidget* parent)
	: QFrame(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setFocus();

	QImage newImage(QSize(600, 600), QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 255));
	m_Image = newImage;
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		m_Reference = event->pos();
		qApp->setOverrideCursor(Qt::ClosedHandCursor);
		setMouseTracking(true);
	}
	
	else if (event->button() == Qt::LeftButton)
	{
		m_LastPoint = event->pos();
		m_Drawing = true;
	}
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		m_Delta += (event->pos() - m_Reference) * 1.0 / m_Scale;
		m_Reference = event->pos();
		update();
	}

	else if ((event->buttons() & Qt::LeftButton) && m_Drawing)
		drawLine(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		qApp->restoreOverrideCursor();
		setMouseTracking(false);
		m_Panning = false;
		update();
	}

	else if (event->button() == Qt::LeftButton && m_Drawing)
	{
		drawLine(event->pos());
		m_Drawing = false;
	}
}

void Canvas::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
		m_Panning = true;
}

void Canvas::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
		m_Panning = false;
}

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.scale(m_Scale, m_Scale);
	painter.translate(m_Delta);

	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect.topLeft(), m_Image, dirtyRect);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
	/*if (width() > m_Image.width() || height() > m_Image.height())
	{
		int newWidth = qMax(width() + 128, m_Image.width());
		int newHeight = qMax(height() + 128, m_Image.height());
		resizeImage(&m_Image, QSize(newWidth, newHeight));
		update();
	}
	*/
	QWidget::resizeEvent(event);
}

void Canvas::drawLine(const QPoint& endPoint)
{
	QPainter painter(&m_Image);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setPen(QPen(m_PenColor, m_PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	painter.drawLine(m_LastPoint - m_Delta, endPoint - m_Delta);
	int radius = (m_PenWidth / 2) + m_PenWidth * 2;

	update();
	//update(QRect(m_LastPoint, endPoint).normalized().adjusted(-radius, -radius, radius, radius));
	
	m_LastPoint = endPoint;
}

void Canvas::resizeImage(QImage* image, const QSize& newSize)
{
	/*if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 255));

	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;*/
}
