#include "pch.h"
#include "Canvas.h"
#include "Data.h"

alp::Canvas::Canvas(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);

	QImage image("res/background-sanity.png");
	m_Background = QPixmap::fromImage(image);
}

void alp::Canvas::resetCanvasLayers(const QSize& size)
{
	m_Scale = int(std::min(size.width(), size.height()) / 4);

	m_Size = size;
	m_Pixmap = QPixmap(size);
	m_Pixmap.fill(Qt::transparent);
}

void alp::Canvas::resetCanvasTransform()
{
	m_Delta = QPointF(0, 0);
	m_Scale = int(std::min(m_Pixmap.size().width(), m_Pixmap.size().height()) / 4);;
	update();
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

	if(currentTool)
		currentTool->mousePressEvent(this, event);
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

	if (currentTool)
		currentTool->mouseMoveEvent(this, event);
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

	if (currentTool)
		currentTool->mouseReleaseEvent(this, event);
}

void alp::Canvas::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		m_Panning = true;
		event->accept();
	}
}

void alp::Canvas::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		qApp->restoreOverrideCursor();
		setMouseTracking(false);
		m_Panning = false;
		event->accept();
	}
}

void alp::Canvas::paintEvent(QPaintEvent* event)
{
	auto dirtyRect = event->rect();
	auto scaledPixmap = m_Pixmap.scaled(m_Size * m_Scale);

	QPainter painter(this);
	painter.translate(rect().center());
	painter.translate(m_Delta);

	painter.setBrush(m_Background);
	painter.setBrushOrigin(scaledPixmap.rect().topLeft());
	painter.drawRect(scaledPixmap.rect());

	if (m_EnableGrid && m_Scale > 2)
	{
		painter.setPen(QPen(QColor(100, 100, 100, 50), 0));

		for (int x = 0; x < scaledPixmap.width(); x += m_Scale)
			painter.drawRect(x, 0, 1, scaledPixmap.height());

		for (int y = 0; y < scaledPixmap.height(); y += m_Scale)
			painter.drawRect(0, y, scaledPixmap.height(), 1);
	}

	painter.drawPixmap(rect().topLeft(), scaledPixmap, scaledPixmap.rect());
}

void alp::Canvas::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0 && m_Scale - 1 <= 16)
	{
		m_Scale += 1;
		update();
	}
	else if (event->angleDelta().y() < 0 && m_Scale - 1 >= 1)
	{
		m_Scale -= 1;
		update();
	}
}
