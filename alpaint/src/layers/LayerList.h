#pragma once
#include "pch.h"
#include "Layer.h"

namespace alp
{
	class LayerList : public QListWidget
	{
		Q_OBJECT

	public:
		LayerList(QWidget* parent) : QListWidget(parent) { }

	protected:
		void dropEvent(QDropEvent* event) override
		{
			QListWidget::dropEvent(event);
			emit onDrop();
		}

	public slots:
		void setIndexBefore(std::vector<std::shared_ptr<Layer>>& layers)
		{
			indexBefore = layers.size() - row(currentItem()) - 1;
		}

		void afterDrop(std::vector<std::shared_ptr<Layer>>& layers)
		{
			int indexAfter = layers.size() - selectedIndexes()[0].row() - 1;
			swapLayerOrder(layers, indexBefore, indexAfter);
		}

	signals:
		void onDrop();

	private:
		int indexBefore = -1;
	};
}