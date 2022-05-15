#include "pch.h"
#include "ProjectDock.h"

alp::ProjectDock::ProjectDock(QWidget* parent, LayerList* layerList, const QSize& size)
	: QDockWidget(parent), m_LayerListRef(layerList)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setBaseSize(parent->width(), parent->height());
	setAllowedAreas(Qt::AllDockWidgetAreas);

	setupTitle();
	setupLayer(size);
	setupCanvas(parent, size);

	m_TabIndex = s_Tab;
	s_Tab++;
}

bool alp::ProjectDock::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_LayerListRef->viewport() && event->type() == QEvent::Drop)
	{
		if (m_Hidden)
			return false;

		m_LayerListRef->setIndexBefore(m_Layers);
	}

	return QWidget::eventFilter(object, event);
}

void alp::ProjectDock::saveNewProject()
{
	//QString initialPath = QDir::currentPath() + "/" + m_ProjectName;

			//auto path = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
			//	tr("PNG Files (*.png);;All Files (*)"));

			//if (path.isEmpty())
			//	return;

			//QFileInfo fileInfo(path);
			//m_ProjectName = fileInfo.baseName();
			//m_ProjectAbsPath = fileInfo.absolutePath();

			//// canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			//
			//m_Modified = false;
			//m_IsDefault = false;

			//updateTitle();

			//--unnamedCount;
}

void alp::ProjectDock::saveChanges()
{
	//if (m_IsDefault)
			//	return;

			//// canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			//m_Modified = false;

			//updateTitle();
}

void alp::ProjectDock::updateTitle()
{
	if (m_Modified)
		setWindowTitle(m_ProjectName + " *");
	else
		setWindowTitle(m_ProjectName);
}

void alp::ProjectDock::showItems()
{
	if (!m_Hidden)
		return;

	for (auto layer : m_Layers)
	{
		LayerWidget* widget = new LayerWidget(m_LayerListRef);
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(widget->sizeHint());

		widget->selectLayer(layer);
		widget->ui.layerName->setText(layer->name);

		m_LayerListRef->addItem(item);
		m_LayerListRef->setItemWidget(item, widget);
	}

	m_LayerListRef->setCurrentRow(m_LayerListRef->count());

	m_Hidden = false;
}

void alp::ProjectDock::hideItems()
{
	if (m_Hidden)
		return;

	m_LastSelected = m_LayerListRef->row(m_LayerListRef->currentItem());
	m_LayerListRef->clear();
	m_Hidden = true;
}

void alp::ProjectDock::deleteSelectedLayer()
{
	if (m_LayerListRef->count() <= 1)
		return;

	m_DefaultLayerAmount--;

	deleteLayer(m_Layers, m_Canvas->getLayer());

	m_Canvas->update();
	m_Canvas->selectLayer(m_Layers.back());

	m_LayerListRef->takeItem(m_LayerListRef->row(m_LayerListRef->currentItem()));

	m_LayerListRef->setCurrentRow(m_LayerListRef->count() - 1);
	auto layer = (LayerWidget*)m_LayerListRef->itemWidget(m_LayerListRef->currentItem());
	layer->updateLayer();
}

void alp::ProjectDock::addNewLayer(const QSize& size)
{
	LayerWidget* widget = new LayerWidget(m_LayerListRef);
	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(widget->sizeHint());

	m_Layers.push_back(createNewLayer(size, "Layer - " + QString::number(m_DefaultLayerAmount)));
	widget->selectLayer(m_Layers.back());
	m_DefaultLayerAmount++;

	m_LayerListRef->insertItem(m_LayerListRef->count(), item);
	m_LayerListRef->setItemWidget(item, widget);

	m_LayerListRef->setCurrentRow(m_LayerListRef->row(item));
}

void alp::ProjectDock::focusLastLayer()
{
	m_LayerListRef->setCurrentRow(m_LastSelected);
}

void alp::ProjectDock::setupTitle()
{
	m_ProjectName = "Untitled - " + QString::number(s_UnnamedCount);
	++s_UnnamedCount;

	setWindowTitle(m_ProjectName);
}

void alp::ProjectDock::setupLayer(const QSize& size)
{
	addNewLayer(size);

	connect(m_LayerListRef, &QListWidget::currentItemChanged, this, [=]() {
		auto layerWidget = (LayerWidget*)m_LayerListRef->itemWidget(m_LayerListRef->currentItem());
		if (layerWidget)
			m_Canvas->selectLayer(layerWidget->layer);
		});

	connect(m_LayerListRef, &LayerList::onDrop, m_LayerListRef, [&]() {
		if (m_Hidden)
			return;

		m_LayerListRef->afterDrop(m_Layers);
		m_Canvas->update();
		});

	m_LayerListRef->viewport()->installEventFilter(this);
}

void alp::ProjectDock::setupCanvas(QWidget* parent, const QSize& size)
{
	m_Canvas = new Canvas(this, size, m_Layers);
	setWidget(m_Canvas);

	connect(m_Canvas, &Canvas::projectModified, this, [&]() {
		setModified(true);
		updateTitle();
		});

	connect(m_Canvas, &Canvas::projectModified, this, [&]() {
		if (!m_LayerListRef->currentItem())
			return;

		LayerWidget* layer = (LayerWidget*)(m_LayerListRef->itemWidget(m_LayerListRef->currentItem()));
		layer->updateLayer();
		});

	m_Canvas->selectLayer(m_Layers.back());
}
