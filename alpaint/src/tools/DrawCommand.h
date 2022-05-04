#pragma once
#include "pch.h"

namespace alp
{
	class DrawCommand : public QUndoCommand
	{
	public:
		DrawCommand(const QPixmap& oldPixmap, QPixmap* pixmap, QUndoCommand* parent = nullptr)
			: QUndoCommand(parent)
		{
			this->m_Pixmap = pixmap;
			this->m_OldPixmap = oldPixmap;
			m_NewPixmap = m_Pixmap->copy(QRect());
		}

	public:
		void undo() override
		{
			*m_Pixmap = m_OldPixmap.copy();
		}

		void redo() override
		{
			*m_Pixmap = m_NewPixmap.copy();
		}

	private:
		QPixmap* m_Pixmap;
		QPixmap m_OldPixmap, m_NewPixmap;
	};
}