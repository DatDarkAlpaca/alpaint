#include "pch.h"
#include "Canvas.h"
#include "Data.h"

alp::Canvas::Canvas(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	m_Pixmap.fill(qRgb(255, 255, 255));
}

void alp::Canvas::resize(const QSize& size)
{
	m_Size = size;
	m_Pixmap = QPixmap(size);
	m_Pixmap.fill(qRgb(255, 255, 255));
}

void alp::Canvas::mousePressEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		m_Reference = event->pos();
		qApp->setOverrideCursor(Qt::ClosedHandCursor);
		setMouseTracking(true);

		return;
	}
	
	if (event->buttons() & (Qt::LeftButton | Qt::RightButton))
	{
		m_LastPoint = event->pos();
		drawLine(event->pos(), event->button() == Qt::RightButton);
		m_Drawing = true;
	}
}

void alp::Canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		m_Delta += (event->pos() - m_Reference);
		m_Reference = event->pos();

		update();

		return;
	}

	if (event->buttons() & (Qt::LeftButton | Qt::RightButton) && m_Drawing)
		drawLine(event->pos(), event->button() == Qt::RightButton);
}

void alp::Canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_Panning)
	{
		qApp->restoreOverrideCursor();
		setMouseTracking(false);
		m_Panning = false;

		update();

		return;
	}

	if (event->buttons() & (Qt::LeftButton | Qt::RightButton) && m_Drawing)
	{
		drawLine(event->pos(), event->button() == Qt::RightButton);
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
	painter.translate(rect().center() - m_Pixmap.rect().center());
	painter.translate(m_Delta);

	auto dirtyRect = event->rect();
	painter.drawPixmap(dirtyRect.topLeft(), m_Pixmap.scaled(m_Size * m_Scale), dirtyRect);
}

void alp::Canvas::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0 && m_Scale <= 16)
	{
		m_Scale *= 2;
		update();
	}
	else if (event->angleDelta().y() < 0 && m_Scale >= 2)
	{
		m_Scale /= 2;
		update();
	}
}

void alp::Canvas::drawLine(const QPoint& endPoint, bool isSecondaryButton = false)
{
	QPainter painter(&m_Pixmap);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, 0);

	QColor usedColor = (!isSecondaryButton) ? primaryColor : secondaryColor;
	
	painter.setPen(QPen(usedColor, m_PenWidth, Qt::SolidLine, Qt::PenCapStyle::SquareCap));

	painter.translate(-m_Delta / m_Scale);
	painter.translate((-rect().width() / 2)  / m_Scale + (m_Pixmap.rect().width() / 2)  / m_Scale,
					  (-rect().height() / 2) / m_Scale + (m_Pixmap.rect().height() / 2) / m_Scale);

	auto x = endPoint.x() / m_Scale;
	auto y = endPoint.y() / m_Scale;

	painter.drawPoint(x, y);
	
	update();

	m_LastPoint = endPoint;
}

void alp::Canvas::resizeImage(QImage* image, const QSize& newSize)
{
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));

	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}
