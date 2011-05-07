#include "options_page.h"
#include "ui_options_page.h"

OptionsPage::OptionsPage(Settings &settings, QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::OptionsPage)
	, m_settings(settings)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

	// Сервер
	int server_index = ui->comboBox_server->findText(m_settings.server());
	if (server_index >= 0)
	{
		ui->comboBox_server->setCurrentIndex(server_index);
	}

	// Прокси
	//NOTE Должен соответствовать
	ui->type->setCurrentIndex(m_settings.proxyType());
}

OptionsPage::~OptionsPage()
{
    delete ui;
}

void OptionsPage::on_type_currentIndexChanged(int index)
{
	switch((WebProxy::Type)index)
	{
	case WebProxy::ManualProxy:
		ui->host->setEnabled(true);
		ui->port->setEnabled(true);
		break;
	case WebProxy::AutoProxy:
	case WebProxy::NoProxy:
		ui->host->setEnabled(false);
		ui->port->setEnabled(false);
		break;
	default:
		Q_ASSERT( 0 );
	}
}

void OptionsPage::on_apply_clicked()
{
	// Установить адрес сервера
	m_settings.setServer(ui->comboBox_server->currentText());

	//NOTE Должен соответствовать
	WebProxy::Type type = (WebProxy::Type)ui->type->currentIndex();
	m_settings.setProxyType(type);

	if (type == WebProxy::ManualProxy)
	{
		QUrl url("http://localhost:8080");
		url.setHost(ui->host->text());
		url.setPort(ui->port->text().toInt());
		m_settings.setProxyUrl(url);
	}

	emit sig_applySettings();
}
