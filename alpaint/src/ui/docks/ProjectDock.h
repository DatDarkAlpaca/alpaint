#pragma once
#include "pch.h"
#include "canvas/Canvas.h"
#include "layers/Layer.h"
#include "layers/LayerWidget.h"
#include "layers/LayerList.h"

namespace alp
{
	class ProjectDock : public QDockWidget
	{
		Q_OBJECT

	public:
		ProjectDock(QWidget* parent, LayerList* layerList, const QSize& size);

		ProjectDock(QWidget* parent, LayerList* layerList, const QString& projectName, const QString& absPath, const std::vector<std::shared_ptr<alp::Layer>>& layer);

	protected:
		bool eventFilter(QObject* object, QEvent* event) override;

	public:
		void saveNewProject();

		void saveChanges();

	public:
		void updateTitle();

		void showItems();

		void hideItems();

		void deleteSelectedLayer();

		void addNewLayer(const QSize& size);

		void focusLastLayer();

	public:
		void setModified(bool value) { m_Modified = value; }

		bool isDefault() const { return m_IsDefault; }

		bool modified() const { return m_Modified; }

		int getIndex() const { return m_TabIndex; }

		Canvas* getCanvas() { return m_Canvas; }
		
		QString getProjectName() const { return m_ProjectName; }

		QString getProjectAbsPath() const { return m_ProjectAbsPath; }

	private:
		void setupTitle();

		void setupLayer(const QSize& size);

		void setupCanvas(QWidget* parent, const QSize& size);

	private:
		void writeProjectData(QFile& file);

	private:
		bool m_IsDefault = true, m_Modified = true;
		QString m_ProjectAbsPath, m_ProjectName;
		inline static size_t s_UnnamedCount = 0;

	private:
		int m_DefaultLayerAmount = 0;
		int m_LastSelected;

	private:
		static inline int s_Tab = 0;
		int m_TabIndex = 0;

	private:
		bool m_Hidden = false;

	private:
		std::vector<std::shared_ptr<Layer>> m_Layers = {};
		Canvas* m_Canvas = nullptr;
		LayerList* m_LayerListRef;
	};
}
