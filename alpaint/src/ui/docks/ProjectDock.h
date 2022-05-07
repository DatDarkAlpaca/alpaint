#pragma once
#include "pch.h"
#include "canvas/Canvas.h"

namespace alp
{
	class ProjectDock : public QDockWidget
	{
		Q_OBJECT

	public:
		ProjectDock(QWidget* parent, Canvas* canvas)
			: QDockWidget(parent), canvas(canvas)
		{
			QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
			setSizePolicy(sizePolicy);

			setBaseSize(parent->width(), parent->height());

			setAllowedAreas(Qt::AllDockWidgetAreas);
			setWidget(canvas);

			m_ProjectName = "Untitled - " + QString::number(unnamedCount);
			++unnamedCount;
			
			setWindowTitle(m_ProjectName);
		}

	public:
		void saveNewProject()
		{
			QString initialPath = QDir::currentPath() + "/" + m_ProjectName;

			auto path = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
				tr("PNG Files (*.png);;All Files (*)"));

			if (path.isEmpty())
				return;

			QFileInfo fileInfo(path);
			m_ProjectName = fileInfo.baseName();
			m_ProjectAbsPath = fileInfo.absolutePath();

			setWindowTitle(m_ProjectName);

			canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			
			m_Modified = false;
			m_IsDefault = false;
		}

		void saveChanges()
		{
			if (m_IsDefault)
				return;

			canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			m_Modified = false;
		}
  
	public:
		Canvas* getCanvas() { return canvas; }

		bool isDefault() const { return m_IsDefault; }

	private:
		inline static size_t unnamedCount = 0;
		bool m_IsDefault = true, m_Modified = true;
		QString m_ProjectAbsPath, m_ProjectName;

	private:
		Canvas* canvas = nullptr;
	};
}
