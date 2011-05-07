#pragma once
#include "data.h"

class StationsModel;

//! Список станций
class StationsPage : public MApplicationPage
{
	Q_OBJECT

public:
    StationsPage();
	~StationsPage();

	//! Показать список с текущей страницы
	void showStations(Station::List stations, QVariantMap result);

	//! Есть ли записи в списке
	bool isEmpty() const;

signals:
	void sig_showStation(Station station);
	void sig_requestPage(int page);

private slots:
	//! Поворот экрана
	void on_orientationChanged(M::Orientation orientation);
	//! Предыдущая страница
	void on_prevPage_clicked();
	//! Следующая страница
	void on_nextPage_clicked();
	//! Клик на элемент списка
	void on_list_itemClicked(const QModelIndex &index);

protected:
	virtual void createContent();

private:
	//! Обновить состояние контролов
	void updateControls(bool enable);

	void requestPage();

private:
	struct StationsPageUi *ui;

	StationsModel *m_model; //!< Модель для отображения

	int m_current_page;   //!< Текущая страница
	int m_num_pages;      //!< Общее кол-во страниц
};
