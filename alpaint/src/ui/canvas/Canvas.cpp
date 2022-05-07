#include "pch.h"
#include "Canvas.h"
#include "ToolHandler.h"
#include "tools/ToolUtils.h"
#include "tools/DrawCommand.h"

alp::Canvas::Canvas(QWidget* parent, QSize size)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);

	m_Background = QImage("res/background-sanity.png");

	resetCanvasLayers(size);
	resetCanvasTransform();

	loadSettings();

	initializeUndoStack();
}

void alp::Canvas::resetCanvasLayers(const QSize& size)
{
	m_Scale = int(std::min(size.width(), size.height()) / 4);

	m_Size = size;
	m_CurrentLayer = QImage(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
	m_CurrentLayer.fill(Qt::transparent);
}

void alp::Canvas::resetCanvasTransform()
{
	m_Delta = QPointF(0, 0);
	m_Scale = int(std::min(m_CurrentLayer.size().width(), m_CurrentLayer.size().height()) / 4);;
	update();
}

void alp::Canvas::resizeCanvas(const QSize& size)
{
	QImage copy = m_CurrentLayer.copy();

	m_Size = size;
	m_CurrentLayer = QImage(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);

	QPainter painter(&m_CurrentLayer);
	painter.setPen(Qt::NoPen);
	painter.setBrush(copy);
	painter.setBrushOrigin(m_CurrentLayer.rect().topLeft());
	painter.drawRect(copy.rect());

	resetCanvasTransform();
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

	if (!ToolHandler::currentTool)
		return;

	ToolHandler::currentTool->setStartPoint(getLayerPoint(event->pos(), rect(), m_Delta, m_Scale));

	m_OldLayer = m_CurrentLayer.copy();
	m_Drawing = true;

	if (ToolHandler::currentTool->type == ToolType::Pencil)
		m_DrawingLine = true;

	update();
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

	if (!ToolHandler::currentTool || !m_Drawing)
		return;

	auto point = getLayerPoint(event->pos(), rect(), m_Delta, m_Scale);

	if (ToolHandler::currentTool->type == ToolType::Line)
	{
		if (m_DrawingLine)
		{
			ToolHandler::tools["pencil"]->draw(m_CurrentLayer, point, event->buttons() & Qt::RightButton);
			m_OldLayer = m_CurrentLayer.copy();
			ToolHandler::currentTool->setStartPoint(point);
			m_DrawingLine = false;
		}
	}
	if (ToolHandler::currentTool->type == ToolType::Line || ToolHandler::currentTool->type == ToolType::Ellipse || ToolHandler::currentTool->type == ToolType::Rect)
	{
		ToolHandler::currentTool->setEndPoint(point);
		m_CurrentLayer = m_OldLayer;
	}

	ToolHandler::currentTool->draw(m_CurrentLayer, point, event->buttons() & Qt::RightButton);
	update();
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

	if (!ToolHandler::currentTool)
		return;

	auto point = getLayerPoint(event->pos(), rect(), m_Delta, m_Scale);

	if (ToolHandler::currentTool->type == ToolType::Line)
		ToolHandler::currentTool->setEndPoint(point);

	ToolHandler::currentTool->draw(m_CurrentLayer, point, event->button() == Qt::RightButton);

	m_Drawing = false;

	if (m_OldLayer != m_CurrentLayer)
		saveDrawCommand();

	update();
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

	QPainter painter(this);
	painter.translate(rect().center());
	painter.translate(m_Delta);
	painter.scale(m_Scale, m_Scale);

	if (m_EnableSanityBackground)
	{
		// Play with composition mode.
		painter.setPen(Qt::NoPen);
		painter.setBrush(m_Background);
		painter.setBrushOrigin(rect().topLeft());
		painter.drawRect(rect());
	}
	
	if (m_EnableGrid && m_Scale > 2)
	{
		QPen pen(QColor(100, 100, 100, 50), 0);
		pen.setCosmetic(true);
		painter.setPen(pen);

		for (int x = 0; x < m_CurrentLayer.width(); ++x)
			painter.drawRect(x, 0, 0, m_CurrentLayer.height());

		for (int y = 0; y < m_CurrentLayer.height(); ++y)
			painter.drawRect(0, y, m_CurrentLayer.width(), 0);
	}
	
	painter.drawImage(rect().topLeft(), m_CurrentLayer, m_CurrentLayer.rect());
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

void alp::Canvas::initializeUndoStack()
{
	m_UndoStack = new QUndoStack();
	m_UndoStack->setUndoLimit(m_UndoLimit);
}

void alp::Canvas::saveDrawCommand()
{
	DrawCommand* command = new DrawCommand(m_OldLayer, &m_CurrentLayer);
	m_UndoStack->push(command);
}

void alp::Canvas::loadSettings()
{
	QSettings settings;
	settings.beginGroup("canvas");
	m_EnableGrid = settings.value("enableGrid", false).toBool();
	m_EnableSanityBackground = settings.value("enableBackground", false).toBool();
	m_UndoLimit = settings.value("undoLimit", 100).toInt();
	settings.endGroup();
}
