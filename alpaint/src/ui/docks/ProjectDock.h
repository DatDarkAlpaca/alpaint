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

	public slots:
		void updateTitle()
		{
			if (m_Modified)
				setWindowTitle(m_ProjectName + " *");
			else
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

			canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			
			m_Modified = false;
			m_IsDefault = false;
		
			updateTitle();

			--unnamedCount;
		}

		void saveChanges()
		{
			if (m_IsDefault)
				return;

			canvas->getPreparedImage().save(m_ProjectAbsPath + "/" + m_ProjectName + ".png");
			m_Modified = false;

			updateTitle();
		}
  
	public:
		Canvas* getCanvas() { return canvas; }

		bool isDefault() const { return m_IsDefault; }

		void setModified(bool value) { m_Modified = value; }

		bool modified() const { return m_Modified; }

	private:
		inline static size_t unnamedCount = 0;
		bool m_IsDefault = true, m_Modified = true;
		QString m_ProjectAbsPath, m_ProjectName;

	private:
		Canvas* canvas = nullptr;
	};
}