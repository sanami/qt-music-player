#pragma once
#include "settings.h"

//! Страница настроек
class OptionsPage : public MApplicationPage
{
    Q_OBJECT

public:
	explicit OptionsPage(Settings &settings);
    ~OptionsPage();

signals:
	//! Настройки изменены
	void sig_applySettings();

private slots:
	//! Обновить настройки из контролов
	void on_apply_clicked();

protected:
	virtual void createContent();

private:
	struct OptionsPageUi *ui;

	Settings &m_settings; //!< Настройки из App
};
