#pragma once
#include "pch.h"

namespace alp
{
	class ToolBarDock : public QDockWidget
	{
		Q_OBJECT

	public:
		ToolBarDock(QWidget* parent)
			: QDockWidget(parent)
		{
			//setTitleBarWidget(new QWidget(this));
		}
	};
}
