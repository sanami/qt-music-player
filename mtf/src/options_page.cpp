#include "options_page.h"

struct OptionsPageUi
{
	MComboBox *comboBox_server;
};

OptionsPage::OptionsPage(Settings &settings)
	: m_settings(settings)
	, ui(new OptionsPageUi)
{
	setObjectName("OptionsPage");
	setTitle("Options");
}

OptionsPage::~OptionsPage()
{
	delete ui;
}

void OptionsPage::createContent()
{
	QGraphicsGridLayout *layout = new QGraphicsGridLayout;
	layout->setSpacing(10);

	int row = 0;
	// Сервер
	//layout->addItem(new MLabel("Server"), row, 0);
	{
		MComboBox *combobox = new MComboBox();
		combobox->setTitle("Server");
//		connect(combobox, SIGNAL(currentIndexChanged(QString)), SLOT(on_comboBox_server_currentIndexChanged(QString)));
		QStringList stringList;
		stringList << "http://music.heroku.com" << "http://192.168.1.128:3000" << "http://192.168.1.14:3000" << "http://localhost:3000";
		combobox->addItems(stringList);
		for(int i=0; combobox->count(); ++i)
		{
			if (combobox->itemText(i) == m_settings.server())
			{
				combobox->setCurrentIndex(i);
				break;
			}
		}
		layout->addItem(combobox, row, 0);
		ui->comboBox_server = combobox;
	}
	++row;
	{
		MButton *btn = new MButton("Apply");
		connect(btn, SIGNAL(clicked()), SLOT(on_apply_clicked()));
		layout->addItem(btn, row, 0);
	}

	centralWidget()->setLayout(layout);
}

void OptionsPage::on_apply_clicked()
{
	// Установить адрес сервера
	m_settings.setServer(ui->comboBox_server->currentText());

	emit sig_applySettings();
}
