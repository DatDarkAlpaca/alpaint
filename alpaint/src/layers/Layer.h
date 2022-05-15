#pragma once
#include "pch.h"

namespace alp
{
	struct Layer
	{
	public:
		Layer(const QString& name = QString(), const QImage& image = QImage(), QPainter::CompositionMode mode = QPainter::CompositionMode_SourceOver)
			: name(name), image(image), blendingMode(mode) { }

	public:
		QString name;
		QImage image;
		QPainter::CompositionMode blendingMode;
	};

	inline std::shared_ptr<Layer> createNewLayer(const QSize& size, const QString& name)
	{
		auto layer = std::make_shared<Layer>(name);
		
		layer->image = QImage(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
		layer->image.fill(Qt::transparent);

		return layer;
	}

	inline void swapLayerOrder(std::vector<std::shared_ptr<Layer>>& layers, int before, int after)
	{
		auto element = layers[before];

		layers.erase(layers.begin() + before);
		layers.insert(layers.begin() + after, element);
	}

	inline void deleteLayer(std::vector<std::shared_ptr<Layer>>& layers, const std::shared_ptr<Layer>& layer)
	{
		for (auto it = layers.begin(); it < layers.end();)
		{
			if (*it == layer)
			{
				it->reset();
				it = layers.erase(it);
				break;
			}
			else
				++it;
		}
	}

	inline void clearLayer(std::vector<std::shared_ptr<Layer>>& layers)
	{
		for (auto it = layers.begin(); it < layers.end();)
		{
			delete it->get();
			it = layers.erase(it);
			++it;
		}
	}
}