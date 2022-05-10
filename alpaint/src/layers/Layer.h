#pragma once
#include "pch.h"

namespace alp
{
	struct Layer
	{
	public:
		Layer(const QImage& image = QImage(), QPainter::CompositionMode mode = QPainter::CompositionMode_SourceOver)
			: image(image), blendingMode(mode) 
		{ }

	public:
		QImage image;
		QPainter::CompositionMode blendingMode;
	};

	inline std::vector<std::shared_ptr<Layer>> layers;

	inline std::shared_ptr<Layer>& createNewLayer(const QSize& size)
	{
		auto layer = std::make_shared<Layer>();
		
		layer->image = QImage(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
		layer->image.fill(Qt::transparent);

		layers.push_back(layer);

		return layers.back();
	}

	inline void swapLayerOrder(int before, int after)
	{
		auto element = layers[before];
		layers.erase(layers.begin() + before);
		layers.insert(layers.begin() + after, element);
	}

	inline int findLayerIndex(const std::shared_ptr<Layer>& layer)
	{
		if (!layer)
			return -1;

		for (int i = 0; i < layers.size(); i++)
		{
			if (layer == layers[i])
				return i;
		}

		return -1;
	}

	inline void swapLayerOrder(const std::shared_ptr<Layer>& layer, int second)
	{
		int index = findLayerIndex(layer);

		if (!layers[second] || !layers[index])
			return;

		std::iter_swap(layers.begin() + index, layers.begin() + second);
	}

	inline void deleteLayer(const std::shared_ptr<Layer>& layer)
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
}