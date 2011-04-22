#include "info_page.h"
#include "ui_info_page.h"

InfoPage::InfoPage(QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::InfoPage)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

InfoPage::~InfoPage()
{
    delete ui;
}

void InfoPage::setStation(QVariantMap station)
{
	// Сохранить данные
	m_station = station;

	ui->streams->clear();
	QStringList info;
	foreach(QString name, station.keys())
	{
		if (name == "streams")
		{
			// Данные в список
			QVariantList streams = station["streams"].toList();
			foreach(QVariant stream_var, streams)
			{
				QVariantMap stream = stream_var.toMap()["stream"].toMap();

				// Информация об аудиопотоке
				QStringList columns;
				columns << stream["bitrate"].toString() + "\n" + stream["codec"].toString();
				columns << stream["url"].toString();

				QTreeWidgetItem *it = new QTreeWidgetItem(columns);
				ui->streams->addTopLevelItem(it);
			}
		}
		else
		{
			// Текстом
			if (name == "name" || name == "id")
			{
				info << QString("%1: %2").arg(name).arg(station[name].toString());
			}
		}
	}

	ui->label->setText(info.join("\n"));
}

void InfoPage::on_streams_itemDoubleClicked(QTreeWidgetItem* it)
{
	QString url = it->text(1); // URL в колонке
	emit sig_openStream(m_station, url);
}

void InfoPage::on_media_messages(QString str)
{
	ui->messages->appendPlainText(str);
	ui->messages->ensureCursorVisible();
}

void InfoPage::on_addToFavorites_clicked()
{
	emit sig_addToFavorites(m_station);
}
