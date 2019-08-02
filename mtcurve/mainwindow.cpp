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
	
	connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(slot_showData(bool)));


	pushButton_limitLine->setToolTip(QString::fromLocal8Bit(tr("柱状图")));
	connect(pushButton_limitLine, SIGNAL(clicked(bool)), this, SLOT(slot_showLimitLine()));

	pushButton_7->setToolTip(QString::fromLocal8Bit(tr("打印")));
	connect(pushButton_7, SIGNAL(clicked(bool)), this, SLOT(slot_print(bool)));

	pushButton_6->setToolTip(QString::fromLocal8Bit(tr("另存为图片")));
	connect(pushButton_6, SIGNAL(clicked(bool)), this, SLOT(slot_savePic(bool)));*/


	BaseCurveDialog *dialog1 = new BaseCurveDialog(this);
	BaseCurveDialog *dialog2 = new BaseCurveDialog(this);
	
	QSize wsize = this->geometry().size();
	dialog1->setMinimumHeight(wsize.height()/2);
	dialog2->setMinimumHeight(wsize.height()/2);
	dialog1->setMaximumHeight(wsize.height());
	dialog2->setMaximumHeight(wsize.height());
	verticalLayout_2->addWidget(dialog1);
	verticalLayout_2->addWidget(dialog2);
	connect(dialog1, SIGNAL(otherHide(bool)), dialog2, SLOT(slot_hide(bool)));
	connect(dialog2, SIGNAL(otherHide(bool)), dialog1, SLOT(slot_hide(bool)));
	
	showMaximized();
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::init()
{	
	
}

void MainWindow::hideWidget(bool _isshow)
{

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


void MainWindow::timeUpdate()
{
	
}

void MainWindow::slot_showData(bool)
{

}

void MainWindow::slot_showLimitLine()
{
	
}

void MainWindow::slot_savePic(bool)
{
	QString gcdir = getenv(SYSTEM_ROOT_DIR);
	QString imageFilePath = QFileDialog::getSaveFileName(this, tr("Save File"),
		gcdir,
		tr("PNG (*.png )"));


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
}

void MainWindow::slot_showCompareLine(bool)
{

	m_bcompare = true;
}

void MainWindow::getInterval(int index)
{
}



