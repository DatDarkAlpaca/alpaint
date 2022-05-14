#pragma once
#include "pch.h"
#include "canvas/Canvas.h"
#include "layers/Layer.h"
#include "layers/LayerWidget.h"
#include "layers/LayerList.h"

namespace alp
{
	class ProjectDock : public QDockWidget
	{
		Q_OBJECT

	public:
		ProjectDock(QWidget* parent, LayerList* layerList, const QSize& size)
			: QDockWidget(parent), layerListRef(layerList)
		{
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
			setBaseSize(parent->width(), parent->height());
			setAllowedAreas(Qt::AllDockWidgetAreas);

			setupTitle();
			setupLayer(size);
			setupCanvas(parent, size);
		}

	public: // Slots
		void updateTitle()
		{
			if (m_Modified)
				setWindowTitle(m_ProjectName + " *");
			else
				setWindowTitle(m_ProjectName);
		}

	public:
		void saveNewProject()
		{
			QString initialPath = QDir::currentPath() + "/" + m_ProjectName;

			auto path = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
				tr("PNG Files (*.png);;All Files (*)"));

			if (path.isEmpty())
				return;

			QFileInfo fileInfo(path);
			m_ProjectName = fileInfo.baseName();
			m_ProjectAbsPath = fileInfo.absolutePath();

			// canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			
			m_Modified = false;
			m_IsDefault = false;
		
			updateTitle();

			--unnamedCount;
		}

		void saveChanges()
		{
			if (m_IsDefault)
				return;

			// canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			m_Modified = false;

			updateTitle();
		}
  
	public:
		void deleteSelectedLayer()
		{
			if (layerListRef->count() <= 1)
				return;

			LayerWidget::decreaseDefaultCount();
			
			deleteLayer(m_Layers, m_Canvas->getLayer());

			m_Canvas->update();
			m_Canvas->selectLayer(m_Layers.back());

			layerListRef->takeItem(layerListRef->row(layerListRef->currentItem()));

			layerListRef->setCurrentRow(layerListRef->count() - 1);
			auto layer = (LayerWidget*)layerListRef->itemWidget(layerListRef->currentItem());
			layer->updateLayer();
		}

		void addLayer(const QSize& size)
		{
			LayerWidget* widget = new LayerWidget(layerListRef);
			QListWidgetItem* item = new QListWidgetItem();
			item->setSizeHint(widget->sizeHint());

			m_Layers.push_back(createNewLayer(size));
			widget->selectLayer(m_Layers.back());

			layerListRef->addItem(item);
			layerListRef->setItemWidget(item, widget);

			layerListRef->setCurrentRow(layerListRef->row(item));
		}

	public:
		Canvas* getCanvas() { return m_Canvas; }

		bool isDefault() const { return m_IsDefault; }

		void setModified(bool value) { m_Modified = value; }

		bool modified() const { return m_Modified; }

	protected:
		bool eventFilter(QObject* object, QEvent* event) override
		{
			if (object == layerListRef->viewport() && event->type() == QEvent::Drop)
				layerListRef->setIndexBefore(m_Layers);
			
			return QWidget::eventFilter(object, event);
		}

	private:
		void setupTitle()
		{
			m_ProjectName = "Untitled - " + QString::number(unnamedCount);
			++unnamedCount;

			setWindowTitle(m_ProjectName);
		}

		void setupLayer(const QSize& size)
		{
			addLayer(size);

			connect(layerListRef, &QListWidget::currentItemChanged, this, [=]() {
				auto layerWidget = (LayerWidget*)layerListRef->itemWidget(layerListRef->currentItem());
				m_Canvas->selectLayer(layerWidget->layer);
			});

			connect(layerListRef, &LayerList::onDrop, layerListRef, [&]() {
				layerListRef->afterDrop(m_Layers);
				m_Canvas->update();
			});

			layerListRef->viewport()->installEventFilter(this);
		}

		void setupCanvas(QWidget* parent, const QSize& size)
		{
			m_Canvas = new Canvas(this, size, m_Layers);
			setWidget(m_Canvas);

			connect(m_Canvas, &Canvas::projectModified, this, [&]() {
				setModified(true);
				updateTitle();
			});

			connect(m_Canvas, &Canvas::projectModified, this, [&]() {
				if (!layerListRef->currentItem())
					return;

				LayerWidget* layer = (LayerWidget*)(layerListRef->itemWidget(layerListRef->currentItem()));
				layer->updateLayer();
			});

			m_Canvas->selectLayer(m_Layers.back());
		}

	private:
		bool m_IsDefault = true, m_Modified = true;
		QString m_ProjectAbsPath, m_ProjectName;
		inline static size_t unnamedCount = 0;

	private:
		std::vector<std::shared_ptr<Layer>> m_Layers = {};
		Canvas* m_Canvas = nullptr;

	private:
		LayerList* layerListRef;
	};
}
