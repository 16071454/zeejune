#include "mainwindow.h"
#include <QSizePolicy>
#include <QVboxLayout>
#include "guibaselib/commontitlebar.h"
#include "guibaselib/appfunctions.h"
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent)
//ui(new Ui::MainWindow)
{
	setupUi(this);

	//setWindowFlags(Qt::FramelessWindowHint);
	//top_widget->setFindMostTopWidget();

	/*top_widget->setTitleText(QString::fromLocal8Bit(tr("历史曲线")));
	top_widget->setBtnVisible(CommonTitleBar::BTN_CLOSE, true);
	top_widget->setBtnVisible(CommonTitleBar::BTN_MAXIMUM, true);
	top_widget->setBtnVisible(CommonTitleBar::BTN_MINIMUM, true);
	top_widget->setBtnVisible(CommonTitleBar::BTN_RESTORE, true);*/

	//setAttribute(Qt::WA_QuitOnClose);

	//initAppSkin();
	/*isBar = false;
	m_curColor = Qt::green;
	m_bvalue = false;
	m_bmark =false;
	m_bcompare = false;
	const QString strImgDir = QString("%1/%2/%3.png").arg(getenv(SYSTEM_ROOT_DIR)).arg("/resource/bmp/mtcurve/").arg("mtcurve");
	this->setWindowIcon(QIcon(QPixmap(strImgDir)));*/
//	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_treeChanged(QTreeWidgetItem*, int)));
  
	//m_fetchData = new mtListThread(this);
	//connect(m_fetchData, SIGNAL(finished()), this, SLOT(finishedThreadSlot()));
	//m_paintThread = new ChartPaintThread(this);
	//connect(m_paintThread, SIGNAL(finished()), this, SLOT(init()));
	
	/*dateTimeEdit->setDateTime(QDateTime::currentDateTime());
	dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit->setCalendarPopup(true);

	dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());
	dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
	dateTimeEdit_2->setCalendarPopup(true);
	dateTimeEdit_2->setVisible(true);
	label_2->setVisible(true);*/
	/*pushButton->setToolTip(QString::fromLocal8Bit(tr("显示曲线")));
	connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(slot_showData(bool)));


	pushButton_limitLine->setToolTip(QString::fromLocal8Bit(tr("柱状图")));
	connect(pushButton_limitLine, SIGNAL(clicked(bool)), this, SLOT(slot_showLimitLine()));

	pushButton_7->setToolTip(QString::fromLocal8Bit(tr("打印")));
	connect(pushButton_7, SIGNAL(clicked(bool)), this, SLOT(slot_print(bool)));

	pushButton_6->setToolTip(QString::fromLocal8Bit(tr("另存为图片")));
	connect(pushButton_6, SIGNAL(clicked(bool)), this, SLOT(slot_savePic(bool)));*/

	
	//connect(comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_treeChanged()));

	//setWindowTitle(QString::fromLocal8Bit(tr("历史曲线")));
	
	//m_tableDialog = new DatatableDialog(0);
	//m_tableDialog->setWindowIcon(QIcon(QPixmap(strImgDir)));
	//m_fetchData->start();

	BaseCurveDialog *dialog1 = new BaseCurveDialog(this);
	BaseCurveDialog *dialog2 = new BaseCurveDialog(this);
	//BaseCurveDialog *dialog3 = new BaseCurveDialog(this);
	QSize wsize = this->geometry().size();
	dialog1->setMinimumHeight(wsize.height()/2);
	dialog2->setMinimumHeight(wsize.height()/2);
	dialog1->setMaximumHeight(wsize.height());
	dialog2->setMaximumHeight(wsize.height());
	verticalLayout_2->addWidget(dialog1);
	verticalLayout_2->addWidget(dialog2);
	connect(dialog1, SIGNAL(otherHide(bool)), dialog2, SLOT(slot_hide(bool)));
	connect(dialog2, SIGNAL(otherHide(bool)), dialog1, SLOT(slot_hide(bool)));
	//verticalLayout_2->addWidget(dialog3);
	//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
	showMaximized();
}

MainWindow::~MainWindow()
{
	//delete m_fetchData;
    //delete ui;
}

void MainWindow::init()
{
	/*m_kdChart = new mtKDchart(this);

	QVBoxLayout* chartLayout = new QVBoxLayout(frame);
	chartLayout->addWidget(m_kdChart);
	m_kdChart->setVisible(false);
	m_paintThread = new ChartPaintThread(m_kdChart);
	m_paintThread->start();*/
	//connect(dateTimeEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(slot_timeChange(QDateTime)));
	//connect(dateTimeEdit_2, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(slot_timeChange(QDateTime)));

	
	//tableWidget->verticalHeader()->setHidden(true);
	//tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	//tableWidget->horizontalHeader()->setStretchLastSection(true);
	//tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//QStringList headers;

	//tableWidget->setColumnCount(4);
	//headers << QString::fromLocal8Bit(tr("生成曲线")) << QString::fromLocal8Bit(tr("名称")) << QString::fromLocal8Bit(tr("颜色"));

	//tableWidget->setHorizontalHeaderLabels(headers);
	//tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//tableWidget->setColumnHidden(3, true);
	//tableWidget->setShowGrid(true);

 //   connect(tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(settingTableChanged(int, int)));
	/*if (ui->fliter_widget->isVisible())
	{
		ui->fliter_widget->hide();
	}*/
	
	
}

void MainWindow::hideWidget(bool _isshow)
{
	/*pushButton->setVisible(_isshow);
	pushButton_3->setVisible(_isshow);
	pushButton_limitLine->setVisible(_isshow);
	pushButton_6->setVisible(_isshow);
	pushButton_7->setVisible(_isshow);
	pushButton_setcurve->setVisible(_isshow);
	dateTimeEdit->setVisible(_isshow);*/
}

void MainWindow::initAppSkin()
{
	//构造图标路径
	QString strMtpDir = CMCParamMgr::getMtpDir();
	QString m_strIconPath = strMtpDir + "/resource/bmp/mtcurve/";

	//读取qss皮肤文件
	QString prodir = QString(getenv(SYSTEM_ROOT_DIR));
	QString curPath = QDir::currentDirPath();
	QFile file(prodir + "/resource/bmp/mtcurve/mtcurve.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = tr(file.readAll());
	styleSheet.replace("%skin%", m_strIconPath.replace('\\', '/'));
	qApp->setStyleSheet(styleSheet);
	file.close();
}

//bool MainWindow::slot_treeChanged(/*QTreeWidgetItem *item, int column*/int row)
//{
//	if (m_selectdialog)
//	{
//		m_selectdialog->setRow(row);
//		m_anaglogIndex = tableWidget->item(row, 3)->text().toInt();
//		m_selectdialog->setlogIndex(m_anaglogIndex);
//		QString cname = comboBox_2->currentText() + ":" + tableWidget->item(row, 1)->text();//add
//		m_selectdialog->setCurveNmae(cname);
//	}
//	//	treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
//	
//		return true;
//}

//void MainWindow::slot_timeChange(QDateTime _datatime)
//{
//	if (m_anaglogIndex == -1)
//	{
//		return;
//	}
//	if (dateTimeEdit->dateTime().toTime_t() > dateTimeEdit_2->dateTime().toTime_t())
//	{
//		//qMsgBoxInfo(QString::fromLocal8Bit(tr("时间设置错误,")), 0, false);
//		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("时间设置错误"), QMessageBox::Ok);
//		return;
//	}

//	if ( dateTimeEdit_2->dateTime().toTime_t() - dateTimeEdit->dateTime().toTime_t()>30*24*60*60)
//	{
//		//qMsgBoxInfo(QString::fromLocal8Bit(tr("时间跨度应在30天内")), 0, false);
//		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("时间跨度应在30天内"), QMessageBox::Ok);
//		return;
//	}


//	KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
//	if (m_diagramlist.size()==0)
//	{
//		if (tableWidget->item(m_row, 0)->checkState()== Qt::Checked)
//		{
//			slot_treeChanged(m_row);
//			return;
//		}

//	}
//	QList<CurvDataStruct> chartData = m_kdChart->getchartDataList();

//	for (int i = 0; i < m_diagramlist.size(); i++)
//		{
//		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);

//		chartData.clear();
//		time_t start_date = dateTimeEdit->dateTime().toTime_t();
//		time_t end_date =  dateTimeEdit_2->dateTime().toTime_t();
//		//time_t end_date = start_date + (24 * 3600*30);
//		vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
//		if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT, templines->getId(), start_date, end_date, interval, vec_results))
//		{
//			cout << __LINE__ << " query failed!" << endl;
//			//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
//			//m_kdChart->setVisible(false);
//			continue;;
//		}

//		fstream fp("test.txt", ios::out);
//		for (int i = 0; i < vec_results.size(); ++i)
//		{
//			fp << "result [" << i << "] : " << vec_results[i]->rowCount() << endl;
//			for (int j = 0; j < vec_results[i]->rowCount(); ++j)
//			{
//				CurvDataStruct _curveData;

//				fp << " [" << j << "] ";
//				_curveData.index = vec_results[i]->value<MT_int64>(0, j);
//				fp << "," << vec_results[i]->value<MT_int64>(0, j) << ",";

//				fp << Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT);
//				_curveData.startTime = QString::fromStdString(Poco::DateTimeFormatter::format(vec_results[i]->value<Poco::DateTime>(1, j), Poco::DateTimeFormat::SORTABLE_FORMAT));
//				fp << "," << vec_results[i]->value<double>(2, j);
//				_curveData._value = vec_results[i]->value<double>(2, j);
//				fp << "," << vec_results[i]->value<MT_int64>(3, j) << endl;
//				chartData.append(_curveData);
//			}
//		}
//		fp.close();

//		if (!chartData.isEmpty())
//		{
//			m_kdChart->setVisible(true);
//			QString cname = comboBox_2->currentText() + ":" ;//add
//			m_kdChart->addLineDiagram(chartData, cname, m_anaglogIndex, start_date, end_date);
//			m_paintThread->setrun(true);

//		}
//		else
//		{
//			//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
//			//m_kdChart->hide();
//			continue;

//		}
//	}

//}

//void MainWindow::finishedThreadSlot()
//{
////	treeWidget->setHeaderHidden(true);
//
//	QMap<int, QString> stationMap = m_fetchData->getSubstation();
//
//	for (int i = 0; i < stationMap.size(); i++)
//	{//
//	//	QTreeWidgetItem* item = new QTreeWidgetItem;
//		QString name = stationMap.value(stationMap.keys().at(i));
//		//item->setText(0, name);
//	//	item->setData(0, Qt::UserRole, stationMap.keys().at(i));
////		treeWidget->addTopLevelItem(item);
//
//		int id = stationMap.keys().at(i);
//		comboBox_station->addItem(name, id);
//		QMultiMap<int, QMap<int, QString> > _gapData = m_fetchData->getGap();
//		QList <QMap<int, QString> > gaplist = _gapData.values(id);
//		for (int i = 0; i < gaplist.size(); i++)
//		{
//			QMap<int, QString> _gapMap = gaplist.at(i);
//			//QTreeWidgetItem* pItem = new QTreeWidgetItem;
//			int index = _gapMap.keys().first();
//			//pItem->setText(0, _gapMap.value(index));
//			//pItem->setData(0, Qt::UserRole, index);
//			//item->addChild(pItem);
//		}
//	}
//	comboBox_station->setCurrentIndex(0);
//}

void MainWindow::timeUpdate()
{
	
}

void MainWindow::slot_showData(bool)
{
	//pushButton->setVisible(false);
	//pushButton_limitLine->setVisible(true);
	////dockWidget->setVisible(true);
	//dateTimeEdit->setVisible(true);
	//pushButton_setcurve->setVisible(true);
	//dateTimeEdit_2->setVisible(true);
	//label->setVisible(true);
	//label_2->setVisible(true);
	//label_3->setVisible(true);
	//comboBox->setVisible(true);
	//isBar = false;
	//m_kdChart->setIsbar(isBar);
	//m_kdChart->changeBarsToLine();
}

void MainWindow::slot_showLimitLine()
{
	//pushButton->setVisible(true);
	//pushButton_limitLine->setVisible(false);
	////dockWidget->setVisible(false);
	//dateTimeEdit->setVisible(false);
	//dateTimeEdit_2->setVisible(false);
	//label->setVisible(false);
	//label_2->setVisible(false);
	//label_3->setVisible(false);
	//comboBox->setVisible(false);
	//pushButton_setcurve->setVisible(false);
	//isBar = true;
	//m_kdChart->setIsbar(isBar);
	//m_kdChart->addBarDiagram();
}

void MainWindow::slot_savePic(bool)
{
	QString gcdir = getenv(SYSTEM_ROOT_DIR);
	QString imageFilePath = QFileDialog::getSaveFileName(this, tr("Save File"),
		gcdir,
		tr("PNG (*.png )"));


	//if (!imageFilePath.isEmpty())
	//{
	//	if (!imageFilePath.contains(".png"))
	//	{
	//		imageFilePath = imageFilePath + QString(".png");
	//	}

	//	QImage pImage = QPixmap::grabWidget(groupBox, frame->rect()).toImage();
	//	pImage.save(imageFilePath);

	//}
	return ;
}

void MainWindow::slot_print(bool)
{
	if (m_pPrinter == NULL)
	{
		m_pPrinter = new QPrinter;
	}

	QPrintDialog dlg(m_pPrinter);
	dlg.setWindowTitle(QObject::tr("打印曲线"));
	//if (dlg.exec() == QDialog::Accepted)
	//{
	//	QPainter painter(m_pPrinter);

	//	QImage pImage = QPixmap::grabWidget(groupBox, frame->rect()).toImage();
	//	QRect rect = painter.viewport();   
	//	QSize size = pImage.size();
	//	size.scale(rect.size(), Qt::KeepAspectRatio);    
	// 
	//	painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
	//	painter.setWindow(pImage.rect());

	//	painter.drawImage(0,0,pImage);

	//}
}

void MainWindow::slot_showCompareLine(bool)
{

	m_bcompare = true;
}

void MainWindow::getInterval(int index)
{
	/*switch (index)
	{
	
	case 0:
		interval = 900;
		break;
	case 1:
		interval = 1800;
		break;
	case 2:
		interval = 2400;
		break;
	case 3:
		interval = 3300;
		break;
	case 4:
		interval = 3600;
		break;
	}
	slot_timeChange(dateTimeEdit->dateTime());*/
}

//void MainWindow::slot_setcombox_2(int index)
//{
//	comboBox_2->clear();
//	QMultiMap<int, QMap<int, QString> > _gapData = m_fetchData->getGap();
//	int id = comboBox_station->itemData(index).toInt();
//	QList <QMap<int, QString> > gaplist = _gapData.values(id);
//	for (int i = 0; i < gaplist.size(); i++)
//	{
//		QMap<int, QString> _gapMap = gaplist.at(i);
//
//		int index1 = _gapMap.keys().first();
//		comboBox_2->addItem(_gapMap.value(index1), index1);
//	}
//    int colum = comboBox_2->model()->columnCount();
//	for (int i = 0; i < comboBox_2->model()->columnCount(); i++)
//	{
//		comboBox_2->model()->sort(i, Qt::AscendingOrder);
//	}
//	comboBox_2->setCurrentIndex(0);
//}
//
//void MainWindow::slot_setcombox_3(int index)
//{
//	tableWidget->setRowCount(0);
//    tableWidget->clearContents();
//    int id = comboBox_2->itemData(index).toInt();
//
//    int substationId = comboBox_station->itemData(comboBox_station->currentIndex()).toInt();
//    QList<RTANALOG_DATA*> _rtanalogList = m_fetchData->getAnalogList();
//	//tableWidget->setVisible(true);
//	for (int i = 0; i < _rtanalogList.size();i++)
//    {
//		RTANALOG_DATA* anadata = _rtanalogList.at(i);
//        if (anadata->m_equipcontainid == id && anadata->m_substationid == substationId)
//        {
//
//            int RowCont;
//            RowCont = tableWidget->rowCount();
//			tableWidget->insertRow(RowCont);//增加一行
//            QTableWidgetItem *checkBox = new QTableWidgetItem();
//            tableWidget->setItem(RowCont, 0, checkBox);
//
//            QString eid = QString("%1").arg(anadata->m_id);
//            tableWidget->setItem(RowCont, 3, new QTableWidgetItem(eid));
//            QString name = QString::fromLocal8Bit(anadata->m_alias);
//            tableWidget->setItem(RowCont, 1, new QTableWidgetItem(name));
//            QPushButton * pbutton = new QPushButton(this);
//            connect(pbutton,SIGNAL(clicked(bool)),this,SLOT(setCurrentLinesColor()));
//            tableWidget->setCellWidget(RowCont, 2, pbutton);
//            QPalette pal = pbutton->palette();
//
//            if(insertLineList.contains(anadata->m_id))
//            {
//                KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
//                QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
//                MyLineDiagram *templines = NULL;
//                while (iter.hasNext())
//                {
//                    templines = (MyLineDiagram *)iter.next();
//					if (templines->getId() == anadata->m_id)
//                    {
//                        break;
//                    }
//                }
//                checkBox->setCheckState(Qt::Checked);
//                pal.setColor(QPalette::Button, templines->getColor());
//                pbutton->setPalette(pal);
//                pbutton->setAutoFillBackground(true);
//                pbutton->setFlat(true);
//                pbutton->setEnabled(true);
//            }
//            else
//            {
//                checkBox->setCheckState(Qt::Unchecked);
//                pal.setColor(QPalette::Button, Qt::gray);
//                pbutton->setPalette(pal);
//                pbutton->setAutoFillBackground(true);
//                pbutton->setFlat(true);
//                pbutton->setEnabled(false);
//            }
//        }
//    }
//
//}
//
//void MainWindow::setCurrentLinesShow(bool isshow,int row)
//{
//	int m_rowIndex = row;
//    int id = tableWidget->item(m_rowIndex,3)->text().toInt();
//	QPushButton *_colorBtn = (QPushButton *)tableWidget->cellWidget(m_rowIndex, 2);
//    if(!isshow)
//	{
//		KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
//		QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
//		MyLineDiagram *templines = NULL;
//		while (iter.hasNext())
//		{
//			templines = (MyLineDiagram *)iter.next();
//			if (templines->getId() == id)
//			{
//				break;
//			}
//		}
//		if (templines==NULL)
//		{
//			return;
//		}
//		templines->setshow(false);
//		QPalette pal = _colorBtn->palette();
//		pal.setColor(QPalette::Button, Qt::gray);
//		_colorBtn->setPalette(pal);
//		_colorBtn->setAutoFillBackground(true);
//		_colorBtn->setFlat(true);
//		_colorBtn->setEnabled(false);
//		m_kdChart->setLines(); 
//		int index = insertLineList.indexOf(id);
//		insertLineList.remove(index);
//		m_paintThread->setrun(true);
//    }
//   else
//    {
//	   if (slot_treeChanged(m_rowIndex))
//	   {
//		   QPalette pal = _colorBtn->palette();
//		   pal.setColor(QPalette::Button, Qt::green);
//		   _colorBtn->setPalette(pal);
//		   _colorBtn->setAutoFillBackground(true);
//		   _colorBtn->setFlat(true);
//		   _colorBtn->setEnabled(true);
//           insertLineList.append(id);
//
//	   }
//	   else
//	   {
//		   tableWidget->item(row, 0)->setCheckState(Qt::Unchecked);
//	   }
//	  
//    }
//}
//
//
//void MainWindow::setCurrentLinesColor()
//{
//    QPushButton *senderObj=qobject_cast<QPushButton*>(sender());
//    KDChart::AbstractDiagramList m_diagramlist = m_kdChart->getCartesianPlane()->diagrams();
//
//    int x = senderObj->frameGeometry().x();
//    int y = senderObj->frameGeometry().y();
//    QModelIndex index = tableWidget->indexAt(QPoint(x, y));
//    int m_rowIndex = index.row();
//    MyLineDiagram *templines =NULL;
//    int id = tableWidget->item(m_rowIndex,3)->text().toInt();
//    QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
//    while (iter.hasNext())
//    {
//       templines = (MyLineDiagram *)iter.next();
//       if(templines->getId()==id)
//       {
//           break;
//       }
//    }
//
//     if(senderObj == NULL)
//       {
//           return;
//       }
//
//    QColorDialog dialog(Qt::green,this);
//    dialog.setOption(QColorDialog::ShowAlphaChannel);
//    if (dialog.exec() == QDialog::Accepted)
//    {
//        QColor m_color = dialog.currentColor();
//        QPalette pal = senderObj->palette();
//        pal.setColor(QPalette::Button, m_color);
//        senderObj->setPalette(pal);
//        senderObj->setAutoFillBackground(true);
//        senderObj->setFlat(true);
//        templines->setColor(m_color);
//		m_paintThread->setrun(true);
//    }
//}
//
//void MainWindow::settingTableChanged(int row, int col)
//{
//	QString ss = tableWidget->item(row, 3)->text();
//
//	if (col==0)
//	{
//		setCurrentLinesShow(tableWidget->item(row,0)->checkState(),row);
//	}
//}
//
//void MainWindow::setCurrentcurveDlg(bool isshow)
//{
//	if (isshow)
//	{
//		frame->setVisible(true);
//		m_selectdialog = (BaseCurveDialog *)sender();
//		return;
//	}
//	if (!dialog1->Isclick() && !dialog2->Isclick() && !dialog3->Isclick())
//	{
//		frame->setVisible(false);
//	}
//	
//	
//}

