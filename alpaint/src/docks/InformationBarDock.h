#pragma once
#include "pch.h"

namespace alp
{
	class InformationBarDock : public QDockWidget
	{
		Q_OBJECT

	public:
		InformationBarDock(QWidget* parent) 
			: QDockWidget(parent)
		{
			setTitleBarWidget(new QWidget(this));
		}
	};
}