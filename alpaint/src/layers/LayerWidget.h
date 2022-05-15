#pragma once
#include "pch.h"
#include "ui_LayerWidget.h"
#include "layers/Layer.h"

namespace alp
{
	class LayerWidget : public QWidget
	{
		Q_OBJECT

	public:
		LayerWidget(QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);
		}

	public:
		void selectLayer(const std::shared_ptr<Layer>& layer)
		{
			this->layer = layer;
			ui.previewImage->setPixmap(QPixmap::fromImage(this->layer->image));
			ui.layerName->setText(layer->name);
		}

		void updateLayer()
		{
			ui.previewImage->setPixmap(QPixmap::fromImage(layer->image));
		}

	public:
		std::shared_ptr<Layer> layer = nullptr;
		
	public:
		::Ui::LayerWidget ui;
	};
}