#include "pch.h"
#include "Data.h"
#include "Canvas.h"
#include "tools/DrawCommand.h"

alp::Canvas::Canvas(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);

	QImage image("res/background-sanity.png");
	m_Background = QPixmap::fromImage(image);

	m_UndoStack = new QUndoStack();
	m_UndoStack->setUndoLimit(UndoLimit);
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

void alp::Canvas::onUndo()
{
	if (!m_UndoStack->canUndo())
		return;

	m_UndoStack->undo();
	update();
}

void alp::Canvas::onRedo()
{
	if (!m_UndoStack->canRedo())
		return;

	m_UndoStack->redo();
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

	if (currentTool)
	{
		currentTool->setStartPoint(event->pos());

		m_OldPixmap = m_Pixmap.copy();
		m_Drawing = true;

		if (currentTool->type == ToolType::Pencil)
			m_DrawingLine = true;
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

	if (currentTool && m_Drawing)
	{
		if (currentTool->type == ToolType::Line)
		{
			if(m_DrawingLine)
			{
				currentTool->setStartPoint(event->pos());
				m_DrawingLine = false;
			}
			else
				currentTool->setEndPoint(event->pos());

			m_Pixmap = m_OldPixmap;
		}

		currentTool->draw(this, event->pos(), event->buttons() & Qt::RightButton);
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

		return;
	}

	if (currentTool)
	{
		if (currentTool->type == ToolType::Line)
			currentTool->setEndPoint(event->pos());

		currentTool->draw(this, event->pos(), event->button() == Qt::RightButton);
					
		m_Drawing = false;

		if (m_OldPixmap.toImage() != m_Pixmap.toImage())
			saveDrawCommand();
	}
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

	if (m_EnableSanityBackground)
	{
		painter.setBrush(m_Background);
		painter.setBrushOrigin(scaledPixmap.rect().topLeft());
		painter.drawRect(scaledPixmap.rect());
	}
	
	if (m_EnableGrid && m_Scale > 2)
	{
		QPen pen(QColor(100, 100, 100, 50), 0);
		pen.setCosmetic(true);
		painter.setPen(pen);

		for (int x = 0; x < scaledPixmap.width(); x += m_Scale)
			painter.drawRect(x, 0, 0, scaledPixmap.height());

		for (int y = 0; y < scaledPixmap.height(); y += m_Scale)
			painter.drawRect(0, y, scaledPixmap.height(), 0);
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

void alp::Canvas::saveDrawCommand()
{
	DrawCommand* command = new DrawCommand(m_OldPixmap, &m_Pixmap);
	m_UndoStack->push(command);
}
