#include "pch.h"
#include "Canvas.h"
#include "Data.h"
#include <iostream>

alp::Canvas::Canvas(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	QImage newImage(QSize(32, 32), QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 255));
	m_Image = newImage;
}

void alp::Canvas::resize(const QSize& size)
{
	resizeImage(&m_Image, size);
}

void alp::Canvas::scale(qreal scale)
{
	m_Scale = scale;
	update();
}

void alp::Canvas::mousePressEvent(QMouseEvent* event)
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
		drawLine(event->pos(), false);
		m_Drawing = true;
	}
	else if (event->button() == Qt::RightButton)
	{
		m_LastPoint = event->pos();
		drawLine(event->pos(), true);
		m_Drawing = true;
	}
}

void alp::Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		m_Delta += (event->pos() - m_Reference) * 1.0 / m_Scale;
		m_Reference = event->pos();
		update();
		return;
	}

	if (m_Drawing)
	{
		if ((event->buttons() & Qt::LeftButton))
			drawLine(event->pos(), false);

		else if ((event->buttons() & Qt::LeftButton))
			drawLine(event->pos(), true);
	}
}

void alp::Canvas::mouseReleaseEvent(QMouseEvent* event)
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
		drawLine(event->pos(), false);
		m_Drawing = false;
	}

	else if (event->button() == Qt::RightButton && m_Drawing)
	{
		drawLine(event->pos(), true);
		m_Drawing = false;
	}
}

void alp::Canvas::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
		m_Panning = true;
}

void alp::Canvas::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		qApp->restoreOverrideCursor();
		setMouseTracking(false);
		m_Panning = false;
	}
}

void alp::Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.scale(m_Scale, m_Scale);
	painter.translate(m_Delta);

	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect.topLeft(), m_Image, dirtyRect);
}

void alp::Canvas::resizeEvent(QResizeEvent* event)
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

void alp::Canvas::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		m_Scale += 1;
		update();
	}
	else if (event->angleDelta().y() < 0)
	{
		m_Scale -= 1;
		update();
	}
}

void alp::Canvas::drawLine(const QPoint& endPoint, bool isSecondaryButton = false)
{
	QPainter painter(&m_Image);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QColor usedColor = (!isSecondaryButton) ? primaryColor : secondaryColor;
	painter.setPen(QPen(usedColor, m_PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		
	painter.translate(-m_Delta);
	painter.scale(1 / m_Scale, 1 / m_Scale);
	painter.drawLine(m_LastPoint, endPoint);
	int radius = (m_PenWidth / 2) + m_PenWidth * 2;

	update();
	//update(QRect(m_LastPoint, endPoint).normalized().adjusted(-radius, -radius, radius, radius));
	
	m_LastPoint = endPoint;
}

void alp::Canvas::resizeImage(QImage* image, const QSize& newSize)
{
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_ARGB32);
	newImage.fill(qRgba(255, 255, 255, 255));

	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}
