#include "mtbasecurvedialog.h"
#include "ui_mtbasecurvedialog.h"
#include <kdchart/KDChartBackgroundAttributes>


BaseCurveDialog::BaseCurveDialog(QWidget *parent) :
    QDialog(parent),
    m_interval(10),
	m_timegap(12 * 60 * 60),
	m_pPrinter(NULL),
	m_fetchData(NULL),
	ismax(false),
	setplength(1),
	sliderinterval(60*60),
    ui(new Ui::BaseCurveDialog)
{
    ui->setupUi(this);
	minute = 14;
	second = 59;
	m_fetchData = new mtListThread;
	connect(m_fetchData, SIGNAL(finished()), this, SLOT(finishedThreadSlot()));
	m_fetchData->start();
	id1 = startTimer(1000);
	QStringList timeList;
	timeList << QString::fromLocal8Bit(tr("12小时")) << QString::fromLocal8Bit(tr("1天")) << QString::fromLocal8Bit(tr("10天")) << QString::fromLocal8Bit(tr("20天")) << QString::fromLocal8Bit(tr("1月"));
	ui->comboBox->insertStringList(timeList);
	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(getInterval(int)));

	connect(ui->pushButton_print, SIGNAL(clicked(bool)), this, SLOT(slot_print(bool)));
	connect(ui->pushButton_pic, SIGNAL(clicked(bool)), this, SLOT(slot_savePic(bool)));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(hideWidget()));
	connect(ui->comboBox_station, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_setcombox_2(int)));
	connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_setcombox_3(int)));
	connect(ui->pushButton_max, SIGNAL(clicked()), this, SLOT(showmax()));

	QString prodir = QString(getenv(SYSTEM_ROOT_DIR));
	QString path = prodir + "/resource/bmp/mtcurve/call.png";
	QString imgpath = QString("QPushButton{background-image: url(%1);}").arg(path);

	imgpath.replace('\\', '/');
	ui->pushButton->setStyleSheet(imgpath);
	ui->comboBox_2->hide();
	ui->comboBox_station->hide();
	ui->tableWidget->hide();
	m_timer = new QTimer(this);

	ui->horizontalSlider->hide();
	m_kdChart = new mtKDchart;

	QHBoxLayout* chartLayout = new QHBoxLayout(ui->frame);
	chartLayout->addWidget(m_kdChart);
	m_kdChart->setVisible(false);
	m_paintThread = new ChartPaintThread(m_kdChart);
	m_paintThread->start();

	ui->tableWidget->verticalHeader()->setHidden(true);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	QStringList headers;

	ui->tableWidget->setColumnCount(4);
	headers << QString::fromLocal8Bit(tr("生成曲线")) << QString::fromLocal8Bit(tr("名称")) << QString::fromLocal8Bit(tr("颜色"));

	ui->tableWidget->setHorizontalHeaderLabels(headers);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget->setColumnHidden(3, true);
	ui->tableWidget->setShowGrid(true);

	connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(settingTableChanged(int, int)));

	m_updateThread = new QUpdateThread(m_kdChart, m_paintThread);
	connect(m_updateThread, SIGNAL(setWaittimer()), this, SLOT(setcountdown()));
	connect(m_updateThread, SIGNAL(setUpdatetimer()), this, SLOT(setupdatetimer()));
	connect(m_updateThread, SIGNAL(setinitial()), this, SLOT(setinitialValue()));
	//m_updateThread->setcuracess(curve_access);
	
	m_updateThread->start();

}

BaseCurveDialog::~BaseCurveDialog()
{
    delete ui;
}

void BaseCurveDialog::getInterval(int index)
{
	switch (index) {
	case 1:
		m_interval = 40;
		m_timegap = 12 * 60 * 60;
		sliderinterval = 60*60;
		break;
	case 2:
		m_timegap = 24 * 60 * 60;
		m_interval = 80;
		sliderinterval = 2*60*60;
		break;
	case 3:
		m_timegap = 10 * 24 * 60 * 60;
		m_interval = 80;
		sliderinterval = 10*60*60;
		break;
	case 4:
		m_interval = 80;
		m_timegap = 20 * 24 * 60 * 60;
		sliderinterval = 10 * 60 * 60;
		break;
	case 5:
		m_interval = 120;
		m_timegap = 30 * 24 * 60 * 60;
		sliderinterval = 15*60*60;
		break;
	default:
		break;
	}
	m_updateThread->setInterval(m_interval);
	m_updateThread->settimegap(m_timegap);
	painCurve();
	time_t end_date = QDateTime::currentDateTime().toTime_t();
	time_t start_date = end_date - m_timegap;
	ui->horizontalSlider->setMinimum(start_date);  // 最小值
	ui->horizontalSlider->setMaximum(end_date);  // 最大值
	ui->horizontalSlider->setSliderPosition(end_date);
	ui->horizontalSlider->setSingleStep(setplength); // 步长
	ui->horizontalSlider->setTickInterval(sliderinterval);  // 设置刻度间隔
	ui->horizontalSlider->setTickPosition(QSlider::TicksAbove);  //刻度在上方
}

void BaseCurveDialog::setlogIndex(int index)
{
	m_anaglogIndex = index;
}

void BaseCurveDialog::setCurveNmae(QString name)
{
	m_curvename = name;
}

bool BaseCurveDialog::slot_treeChanged(/*QTreeWidgetItem *item, int column*/int row)
{
		m_anaglogIndex = ui->tableWidget->item(row, 3)->text().toInt();
		setlogIndex(m_anaglogIndex);
		m_updateThread->setAnaglogIndex(m_anaglogIndex);
		QString cname = ui->comboBox_2->currentText() + ":" + ui->tableWidget->item(row, 1)->text();//add
		setCurveNmae(cname);
		
		return painCurve();
}

void BaseCurveDialog::slot_timeChange()
{
	//if (m_anaglogIndex == -1)
	//{
	//	return;
	//}

	//KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
	//if (m_diagramlist.size() == 0)
	//{
	//	return;
	//	/*if (ui->tableWidget->item(m_row, 0)->checkState() == Qt::Checked)
	//	{
	//	slot_treeChanged(m_row);
	//	return;
	//	}*/

	//}
	//QList<CurvDataStruct> chartData = m_kdChart->getchartDataList();

	//for (int i = 0; i < m_diagramlist.size(); i++)
	//{
	//	MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);

	//	chartData.clear();
	//	time_t end_date = QDateTime::currentDateTime().toTime_t();
	//	time_t start_date = end_date - m_interval;
	//	vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
	//	if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT, templines->getId(), start_date, end_date, m_interval, vec_results))
	//	{
	//		cout << __LINE__ << " query failed!" << endl;
	//		//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
	//		//m_kdChart->setVisible(false);
	//		continue;;
	//	}

	//	fstream fp("test.txt", ios::out);
	//	for (int i = 0; i < vec_results.size(); ++i)
	//	{
	//		fp << "result [" << i << "] : " << vec_results[i]->rowCount() << endl;
	//		for (int j = 0; j < vec_results[i]->rowCount(); ++j)
	//		{
	//			CurvDataStruct _curveData;

	//			fp << " [" << j << "] ";
	//			_curveData.index = vec_results[i]->value<MT_int64>(0, j);
	//			fp << "," << vec_results[i]->value<MT_int64>(0, j) << ",";

	//			fp << Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT);
	//			_curveData.startTime = QString::fromStdString(Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT));
	//			fp << "," << vec_results[i]->value<double>(2, j);
	//			_curveData._value = vec_results[i]->value<double>(2, j);
	//			fp << "," << vec_results[i]->value<MT_int64>(3, j) << endl;
	//			chartData.append(_curveData);
	//		}
	//	}
	//	fp.close();

	//	if (!chartData.isEmpty())
	//	{
	//		m_kdChart->setVisible(true);
	//		QString cname = ui->comboBox_2->currentText() + ":";//add
	//		m_kdChart->addLineDiagram(chartData, cname, m_anaglogIndex, start_date, end_date);
	//		m_paintThread->setrun(true);

	//	}
	//	else
	//	{
	//		//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
	//		//m_kdChart->hide();
	//		continue;

	//	}
	//}

}

void BaseCurveDialog::finishedThreadSlot()
{
	
		QMap<int, QString> stationMap = m_fetchData->getSubstation();
	
		for (int i = 0; i < stationMap.size(); i++)
		{
			QString name = stationMap.value(stationMap.keys().at(i));
			int id = stationMap.keys().at(i);
			ui->comboBox_station->addItem(name, id);
			QMultiMap<int, QMap<int, QString> > _gapData = m_fetchData->getGap();
			QList <QMap<int, QString> > gaplist = _gapData.values(id);
			for (int i = 0; i < gaplist.size(); i++)
			{
				QMap<int, QString> _gapMap = gaplist.at(i);
			
				int index = _gapMap.keys().first();
			}
		}
		ui->comboBox_station->setCurrentIndex(0);
}

void BaseCurveDialog::slot_setcombox_2(int index)
{
	ui->comboBox_2->clear();
	QMultiMap<int, QMap<int, QString> > _gapData = m_fetchData->getGap();
	int id = ui->comboBox_station->itemData(index).toInt();
	QList <QMap<int, QString> > gaplist = _gapData.values(id);
	for (int i = 0; i < gaplist.size(); i++)
	{
		QMap<int, QString> _gapMap = gaplist.at(i);

		int index1 = _gapMap.keys().first();
		ui->comboBox_2->addItem(_gapMap.value(index1), index1);
	}
	int colum = ui->comboBox_2->model()->columnCount();
	for (int i = 0; i < ui->comboBox_2->model()->columnCount(); i++)
	{
		ui->comboBox_2->model()->sort(i, Qt::AscendingOrder);
	}
	ui->comboBox_2->setCurrentIndex(0);
}

void BaseCurveDialog::slot_setcombox_3(int index)
{
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->clearContents();
	int id = ui->comboBox_2->itemData(index).toInt();
	m_updateThread->setcname(ui->comboBox_2->currentText());

	int substationId = ui->comboBox_station->itemData(ui->comboBox_station->currentIndex()).toInt();
	QList<RTANALOG_DATA*> _rtanalogList = m_fetchData->getAnalogList();
	for (int i = 0; i < _rtanalogList.size(); i++)
	{
		RTANALOG_DATA* anadata = _rtanalogList.at(i);
		if (anadata->m_equipcontainid == id && anadata->m_substationid == substationId)
		{

			int RowCont;
			RowCont = ui->tableWidget->rowCount();
			ui->tableWidget->insertRow(RowCont);//增加一行
			QTableWidgetItem *checkBox = new QTableWidgetItem();
			ui->tableWidget->setItem(RowCont, 0, checkBox);

			QString eid = QString("%1").arg(anadata->m_id);
			ui->tableWidget->setItem(RowCont, 3, new QTableWidgetItem(eid));
			QString name = QString::fromLocal8Bit(anadata->m_alias);
			ui->tableWidget->setItem(RowCont, 1, new QTableWidgetItem(name));
			QPushButton * pbutton = new QPushButton(this);
			connect(pbutton, SIGNAL(clicked(bool)), this, SLOT(setCurrentLinesColor()));
			ui->tableWidget->setCellWidget(RowCont, 2, pbutton);
			QPalette pal = pbutton->palette();

			if (insertLineList.contains(anadata->m_id))
			{
				KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
				QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
				MyLineDiagram *templines = NULL;
				while (iter.hasNext())
				{
					templines = (MyLineDiagram *)iter.next();
					if (templines->getId() == anadata->m_id)
					{
						break;
					}
				}
				checkBox->setCheckState(Qt::Checked);
				pal.setColor(QPalette::Button, templines->getColor());
				pbutton->setPalette(pal);
				pbutton->setAutoFillBackground(true);
				pbutton->setFlat(true);
				pbutton->setEnabled(true);
			}
			else
			{
				checkBox->setCheckState(Qt::Unchecked);
				insertLineList.remove(id);
				pal.setColor(QPalette::Button, Qt::gray);
				pbutton->setPalette(pal);
				pbutton->setAutoFillBackground(true);
				pbutton->setFlat(true);
				pbutton->setEnabled(false);
				
			}
		}
	}

}

void BaseCurveDialog::setCurrentLinesShow(bool isshow, int row)
{
	int m_rowIndex = row;
	int id = ui->tableWidget->item(m_rowIndex, 3)->text().toInt();
	QPushButton *_colorBtn = (QPushButton *)ui->tableWidget->cellWidget(m_rowIndex, 2);
	if (!isshow)
	{
		KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
		QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
		MyLineDiagram *templines = NULL;
		while (iter.hasNext())
		{
			templines = (MyLineDiagram *)iter.next();
			if (templines->getId() == id)
			{
				break;
			}
		}
		if (templines == NULL)
		{
			return;
		}
		templines->setshow(false);
		QPalette pal = _colorBtn->palette();
		pal.setColor(QPalette::Button, Qt::gray);
		_colorBtn->setPalette(pal);
		_colorBtn->setAutoFillBackground(true);
		_colorBtn->setFlat(true);
		_colorBtn->setEnabled(false);
		insertLineList.remove(id);
		m_kdChart->setLines();
		m_paintThread->setrun(true);
	}
	else
	{
		if (!slot_treeChanged(m_rowIndex))
		{
			ui->tableWidget->item(row, 0)->setCheckState(Qt::Unchecked);
			insertLineList.remove(id);
		}
		else
		{
			QPalette pal = _colorBtn->palette();
			pal.setColor(QPalette::Button, Qt::green);
			_colorBtn->setPalette(pal);
			_colorBtn->setAutoFillBackground(true);
			_colorBtn->setFlat(true);
			_colorBtn->setEnabled(true);
			insertLineList.append(id);
		}

	}
}


void BaseCurveDialog::setCurrentLinesColor()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();

	int x = senderObj->frameGeometry().x();
	int y = senderObj->frameGeometry().y();
	QModelIndex index = ui->tableWidget->indexAt(QPoint(x, y));
	int m_rowIndex = index.row();
	MyLineDiagram *templines = NULL;
	int id = ui->tableWidget->item(m_rowIndex, 3)->text().toInt();
	QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
	while (iter.hasNext())
	{
		templines = (MyLineDiagram *)iter.next();
		if (templines->getId() == id)
		{
			break;
		}
	}

	if (senderObj == NULL)
	{
		return;
	}

	QColorDialog dialog(Qt::green, this);
	dialog.setOption(QColorDialog::ShowAlphaChannel);
	if (dialog.exec() == QDialog::Accepted)
	{
		QColor m_color = dialog.currentColor();
		QPalette pal = senderObj->palette();
		pal.setColor(QPalette::Button, m_color);
		senderObj->setPalette(pal);
		senderObj->setAutoFillBackground(true);
		senderObj->setFlat(true);
		templines->setColor(m_color);
		m_paintThread->setrun(true);
	}
}

void BaseCurveDialog::hideWidget()
{
	QString prodir = QString(getenv(SYSTEM_ROOT_DIR));
	
	if (ui->tableWidget->isVisible())
	{
		ui->comboBox_2->hide();
		ui->comboBox_station->hide();
		ui->tableWidget->hide();
		QString path = prodir + "/resource/bmp/mtcurve/call.png";
		QString imgpath = QString("QPushButton{background-image: url(%1);}").arg(path);
		imgpath.replace('\\', '/');
		ui->pushButton->setStyleSheet(imgpath);
	}
	else
	{
		ui->comboBox_2->setVisible(true);
		ui->comboBox_station->setVisible(true);
		ui->tableWidget->setVisible(true);
		QString path = prodir + "/resource/bmp/mtcurve/center.png";
		QString imgpath = QString("QPushButton{background-image: url(%1);}").arg(path);
		imgpath.replace('\\', '/');
		ui->pushButton->setStyleSheet(imgpath);
	}
}

void BaseCurveDialog::showmax()
{
	if (ismax)
	{

		emit otherHide(false);
		showNormal();
		ismax = false;
		ui->pushButton_max->setText(QString::fromLocal8Bit(tr("最大化")));
	}
	else
	{
		emit otherHide(true);
		showMaximized();
		ismax = true;
		ui->pushButton_max->setText(QString::fromLocal8Bit(tr("还原")));
	}
	
}

void BaseCurveDialog::slot_hide(bool isdhow)
{
	if (isdhow)
	{
		hide();
	}
	else
		setVisible(true);
}

void BaseCurveDialog::setcountdown()
{
	
}

void BaseCurveDialog::setupdatetimer()
{
	minute = 14;
	second = 59;
	ui->label_updatetime->setText(QString::fromLocal8Bit(tr("更新")));
	
}

void BaseCurveDialog::setinitialValue()
{
	minute = 14;
	second = 59;
}

void BaseCurveDialog::timerEvent(QTimerEvent * t)
{
	int id = t->timerId();
	KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
	if (id == id1 && m_diagramlist.size()>0)
	{
		second--;
		if (second<=0)
		{
			minute--;
			second = 59;
		}
		if (minute<=0)
		{
			minute = 0;
		}
		QString text = QString(QString::fromLocal8Bit(tr("更新时间:%1:%2"))).arg(minute).arg(second);
		ui->label_updatetime->setText(text);
	}
}

void BaseCurveDialog::settingTableChanged(int row, int col)
{
	QString ss = ui->tableWidget->item(row, 3)->text();

	if (col == 0)
	{
		setCurrentLinesShow(ui->tableWidget->item(row, 0)->checkState(), row);
	}
}

void BaseCurveDialog::slot_savePic(bool)
{
	QString gcdir = getenv(SYSTEM_ROOT_DIR);
	QString imageFilePath = QFileDialog::getSaveFileName(this, tr("Save File"),
		gcdir,
		tr("PNG (*.png )"));


	if (!imageFilePath.isEmpty())
	{
		if (!imageFilePath.contains(".png"))
		{
			imageFilePath = imageFilePath + QString(".png");
		}

		QImage pImage = QPixmap::grabWidget(this, this->rect()).toImage();
		pImage.save(imageFilePath);

	}
	return;
}

void BaseCurveDialog::slot_print(bool)
{
	if (m_pPrinter == NULL)
	{
		m_pPrinter = new QPrinter;
	}

	QPrintDialog dlg(m_pPrinter);
	dlg.setWindowTitle(QObject::tr("打印曲线"));
	if (dlg.exec() == QDialog::Accepted)
	{
		QPainter painter(m_pPrinter);

		QImage pImage = QPixmap::grabWidget(this, this->rect()).toImage();
		QRect rect = painter.viewport();
		QSize size = pImage.size();
		size.scale(rect.size(), Qt::KeepAspectRatio);

		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(pImage.rect());

		painter.drawImage(0, 0, pImage);

	}
}

bool BaseCurveDialog::painCurve(time_t start_date, time_t end_date)
{

    if (m_kdChart->getCartesianPlane()->diagrams().size() == 3)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("最多三条数据"), QMessageBox::Ok);
        return false;
    }
	KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
	if (m_diagramlist.size() != 0)
	{

		for (int i = 0; i < m_diagramlist.size(); i++)
		{
			MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);
			
			QList<CurvDataStruct> chartData = m_kdChart->getchartDataList();

			chartData.clear();

			time_t end_date = QDateTime::currentDateTime().toTime_t();
			time_t start_date = end_date - m_timegap;
			vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
			int indexid = templines->getId();

			if (curve_access.init() == false)
			{
				cout << __LINE__ << " error!" << endl;
				return false;
			}
			if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT, indexid, start_date, end_date, 1, vec_results))
			{
				cout << __LINE__ << " query failed!" << endl;
				//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
				//m_kdChart->setVisible(false);
				continue;;
			}

			fstream fp("test.txt", ios::out);
			for (int i = 0; i < vec_results.size(); ++i)
			{
				fp << "result [" << i << "] : " << vec_results[i]->rowCount() << endl;
				for (int j = 0; j < vec_results[i]->rowCount(); ++j)
				{
					CurvDataStruct _curveData;

					fp << " [" << j << "] ";
					_curveData.index = vec_results[i]->value<MT_int64>(0, j);
					fp << "," << vec_results[i]->value<MT_int64>(0, j) << ",";

					fp << Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT);
					_curveData.startTime = QString::fromStdString(Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT));
					fp << "," << vec_results[i]->value<double>(2, j);
					_curveData._value = vec_results[i]->value<double>(2, j);
					fp << "," << vec_results[i]->value<MT_int64>(3, j) << endl;
					chartData.append(_curveData);
				}
			}
			fp.close();

			if (!chartData.isEmpty())
			{
				m_kdChart->setVisible(true);
				m_kdChart->addLineDiagram(chartData, templines->objectName(), indexid, start_date, end_date);
				m_paintThread->setrun(true);

			}
			else
			{
				continue;

			}
		}
	}

	if (!insertLineList.contains(m_anaglogIndex))
	{

		QList<CurvDataStruct> chartData = m_kdChart->getchartDataList();;
		chartData.clear();

		if (curve_access.init() == false)
		{
			cout << __LINE__ << " error!" << endl;
			return false;
		}
		//INSERT INTO hdr_analoginput20190419(ID,HDTIME,VALUE,QUALITYCODE) VALUES(2817,"2019-04-19 13：28：40"，0.000000，128);
		end_date = QDateTime::currentDateTime().toTime_t();
		start_date = end_date - m_timegap;

		vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
		/*QString ss = QString("%1,%2,%3,%4").arg(start_date).arg(end_date).arg(m_interval).arg(m_anaglogIndex);
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), ss, QMessageBox::Ok);*/
		if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT, m_anaglogIndex, start_date, end_date, 1, vec_results))
		{
			cout << __LINE__ << " query failed!" << endl;

			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
			return false;
		}
		fstream fp("test.txt", ios::out);
		for (int i = 0; i < vec_results.size(); ++i)
		{
			fp << "result [" << i << "] : " << vec_results[i]->rowCount() << endl;
			for (int j = 0; j < vec_results[i]->rowCount(); ++j)
			{
				CurvDataStruct _curveData;

				fp << " [" << j << "] ";
				_curveData.index = vec_results[i]->value<MT_int64>(0, j);
				fp << "," << vec_results[i]->value<MT_int64>(0, j) << ",";

				fp << Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT);
				_curveData.startTime = QString::fromStdString(Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT));
				fp << "," << vec_results[i]->value<double>(2, j);
				_curveData._value = vec_results[i]->value<double>(2, j);
				fp << "," << vec_results[i]->value<MT_int64>(3, j) << endl;
				chartData.append(_curveData);
			}
		}
		fp.close();

		if (!chartData.isEmpty())
		{
			m_kdChart->setVisible(true);
			m_kdChart->addLineDiagram(chartData, m_curvename, m_anaglogIndex, start_date, end_date);
			m_paintThread->setrun(true);
		}
		else
		{
			cout << __LINE__ << " query failed!" << endl;
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
			return false;
		}
	}
		return true;
}

void BaseCurveDialog::on_pushButton_slider_clicked()
{
    if(ui->horizontalSlider->isVisible())
    {
        QString prodir = QString(getenv(SYSTEM_ROOT_DIR));
        QString path = prodir + "/resource/bmp/mtcurve/call.png";
        QString imgpath = QString("QPushButton{background-image: url(%1);}").arg(path);

        imgpath.replace('\\', '/');
        ui->pushButton_slider->setStyleSheet(imgpath);
        ui->horizontalSlider->hide();
		this->painCurve();
    }
    else
    {
		KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
		if (m_diagramlist.size() == 0)
		{
			return;
		}
        QString prodir = QString(getenv(SYSTEM_ROOT_DIR));
        QString path = prodir + "/resource/bmp/mtcurve/call.png";
        QString imgpath = QString("QPushButton{background-image: url(%1);}").arg(path);

        imgpath.replace('\\', '/');
        ui->horizontalSlider->setVisible(true);
        ui->pushButton_slider->setStyleSheet(imgpath);
		time_t end_date = QDateTime::currentDateTime().toTime_t();
		time_t start_date = end_date - m_timegap;
		ui->horizontalSlider->setMinimum(start_date);  // 最小值
		ui->horizontalSlider->setMaximum(end_date);  // 最大值
        ui->horizontalSlider->setSliderPosition(end_date);
		ui->horizontalSlider->setSingleStep(setplength); // 步长
		ui->horizontalSlider->setTickInterval(sliderinterval);  // 设置刻度间隔
        ui->horizontalSlider->setTickPosition(QSlider::TicksAbove);  //刻度在上方

    }
}

void BaseCurveDialog::on_horizontalSlider_valueChanged(int value)
{
    int a =66;

}

void BaseCurveDialog::on_horizontalSlider_rangeChanged(int min, int max)
{

}

void BaseCurveDialog::on_horizontalSlider_sliderMoved(int position)
{
	if (ui->horizontalSlider->maximum() - position < 15)
	{
		return;
	}
	time_t end_date = QDateTime::currentDateTime().toTime_t();
	time_t start_date = position;

	KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
	if (m_diagramlist.size() == 0)
	{
		return;
	}

	for (int i = 0; i < m_diagramlist.size(); i++)
	{
		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);
		QList<CurvDataStruct> chartData = m_kdChart->getchartDataList();

		chartData.clear();

		vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
		int indexid = templines->getId();

		if (curve_access.init() == false)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("init"), QMessageBox::Ok);
			cout << __LINE__ << " error!" << endl;
			return;
		}
		if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT, indexid, start_date, end_date, 1, vec_results))
		{
			cout << __LINE__ << " query failed!" << endl;
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
			//m_kdChart->setVisible(false);
			continue;;
		}

		fstream fp("test.txt", ios::out);
		for (int i = 0; i < vec_results.size(); ++i)
		{
			fp << "result [" << i << "] : " << vec_results[i]->rowCount() << endl;
			for (int j = 0; j < vec_results[i]->rowCount(); ++j)
			{
				CurvDataStruct _curveData;

				fp << " [" << j << "] ";
				_curveData.index = vec_results[i]->value<MT_int64>(0, j);
				fp << "," << vec_results[i]->value<MT_int64>(0, j) << ",";

				fp << Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT);
				_curveData.startTime = QString::fromStdString(Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT));
				fp << "," << vec_results[i]->value<double>(2, j);
				_curveData._value = vec_results[i]->value<double>(2, j);
				fp << "," << vec_results[i]->value<MT_int64>(3, j) << endl;
				chartData.append(_curveData);
			}
		}
		fp.close();

		if (!chartData.isEmpty())
		{
			m_kdChart->setVisible(true);
			m_kdChart->addLineDiagram(chartData, ui->comboBox_2->currentText(), indexid, start_date, end_date);
			m_paintThread->setrun(true);

		}
	}
}

void BaseCurveDialog::on_horizontalSlider_sliderPressed()
{
	m_kdChart->setcanpaint(false);
}

void BaseCurveDialog::on_horizontalSlider_sliderReleased()
{
	m_kdChart->setcanpaint(true);
}
