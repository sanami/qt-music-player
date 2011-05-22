#include "station_page.h"
#include "ui_station_page.h"

StationPage::StationPage(QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::StationPage)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

	hide();
}

StationPage::~StationPage()
{
    delete ui;
}

void StationPage::setStation(Station station)
{
	// Сохранить данные
	m_station = station;

	ui->streams->clear();

	// Данные в список
	foreach(Stream stream, m_station.streams())
	{
		// Информация об аудиопотоке
		QStringList columns;
		columns << stream.bitrate() + "\n" + stream.codec();
		columns << stream.url();

		QTreeWidgetItem *it = new QTreeWidgetItem(columns);
		if (stream.status() != 1)
			it->setFlags(Qt::NoItemFlags);
		ui->streams->addTopLevelItem(it);
	}

	// Текстом
	QStringList info;
//	foreach(QString name, station.data.keys())
//	{
//		if (name == "name" || name == "id")
//		{
//			info << QString("%1: %2").arg(name).arg(station.data[name].toString());
//		}
//	}
	info << QString("Id: %1").arg(m_station.id());
	info << QString("Name: %1").arg(m_station.name());

	ui->label->setText(info.join("\n"));
}

void StationPage::on_streams_itemDoubleClicked(QTreeWidgetItem* it)
{
	QString url = it->text(1); // URL в колонке
	emit sig_openStream(m_station, url);
}

void StationPage::on_media_messages(QString str)
{
	ui->messages->appendPlainText(str);
	ui->messages->ensureCursorVisible();
}

void StationPage::on_addToFavorites_clicked()
{
	emit sig_addToFavorites(m_station);
}
