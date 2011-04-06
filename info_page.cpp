#include "info_page.h"
#include "ui_info_page.h"
#include "media.h"

InfoPage::InfoPage(QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::InfoPage)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
//	setWindowFlags(windowFlags() | Qt::Window);
#endif

	m_media = new Media(this);
}

InfoPage::~InfoPage()
{
    delete ui;
	delete m_media;
}

void InfoPage::setStation(QVariantMap station)
{
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
			info << QString("%1: %2").arg(name).arg(station[name].toString());
		}
	}

	ui->label->setText(info.join("\n"));
}

void InfoPage::on_streams_itemDoubleClicked(QTreeWidgetItem* it)
{
	QString url = it->text(1); // URL в колонке
	m_media->open(url);
}
