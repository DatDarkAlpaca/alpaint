#pragma once

namespace alp
{
	struct DocumentData
	{
		DocumentData(QSize size)
			: documentSize(size) { }
		DocumentData() = default;

		QSize documentSize;
	};
}