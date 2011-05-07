#pragma once
#include <QWidget>
#include "settings.h"

namespace Ui {
    class OptionsPage;
}

//! Страница настроек
class OptionsPage : public QWidget
{
    Q_OBJECT

public:
	explicit OptionsPage(Settings &settings, QWidget *parent = 0);
    ~OptionsPage();

signals:
	//! Настройки изменены
	void sig_applySettings();

private slots:
	void on_type_currentIndexChanged(int index);

	//! Обновить настройки из контролов
	void on_apply_clicked();

private:
    Ui::OptionsPage *ui;

	Settings &m_settings; //!< Настройки из App
};
