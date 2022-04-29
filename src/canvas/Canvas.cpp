#include "pch.h"
#include "Canvas.h"

Canvas::Canvas(QWidget* parent)
	: QFrame(parent)
{
	setAttribute(Qt::WA_StaticContents);
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_LastPoint = event->pos();
		m_Drawing = true;
	}
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if ((event->buttons() & Qt::LeftButton) && m_Drawing)
		drawLine(event->pos());
}

void Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && m_Drawing)
	{
		drawLine(event->pos());
		m_Drawing = false;
	}
}

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, m_Image, dirtyRect);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
	if (width() > m_Image.width() || height() > m_Image.height())
	{
		int newWidth = qMax(width() + 128, m_Image.width());
		int newHeight = qMax(height() + 128, m_Image.height());
		resizeImage(&m_Image, QSize(newWidth, newHeight));
		update();
	}

	QWidget::resizeEvent(event);
}

void Canvas::drawLine(const QPoint& endPoint)
{
	QPainter painter(&m_Image);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setPen(QPen(m_PenColor, m_PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	painter.drawLine(m_LastPoint, endPoint);
	int radius = (m_PenWidth / 2) + m_PenWidth * 2;

	update(QRect(m_LastPoint, endPoint).normalized().adjusted(-radius, -radius, radius, radius));

	m_LastPoint = endPoint;
}

void Canvas::resizeImage(QImage* image, const QSize& newSize)
{
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 0));

	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}
