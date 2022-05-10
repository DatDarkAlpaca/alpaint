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
			indexBefore = layers.size() - row(currentItem()) - 1;

			QListWidget::dropEvent(event);
			event->accept();
			emit onDrop(event);
		}

	public slots:
		void afterDrop(QDropEvent* event)
		{
			int indexAfter = layers.size() - selectedIndexes()[0].row() - 1;			
			swapLayerOrder(indexBefore, indexAfter);
		}

	signals:
		void onDrop(QDropEvent* event);

	private:
		int indexBefore = -1;
	};
}