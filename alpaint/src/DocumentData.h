#pragma once

namespace alp
{
	struct DocumentData
	{
		DocumentData(int width, int height)
			: documentWidth(width), documentHeight(height) { }
		DocumentData() = default;

		int documentWidth, documentHeight;
	};
}