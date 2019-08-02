#include "datatabledialog.h"
#include "ui_datatabledialog.h"
#include <kdchart/KDChartAbstractDiagram>
#include <kdchart/KDChartLineDiagram>
#include "mylinediagram.h"
#include "guibaselib/commontitlebar.h"

DatatableDialog::DatatableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatatableDialog)
{
    ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	
	ui->top_widget->setFindMostTopWidget();

	ui->top_widget->setTitleText(QString::fromLocal8Bit(tr("数据显示")));
	ui->top_widget->setBtnVisible(CommonTitleBar::BTN_CLOSE, true);
	ui->top_widget->setBtnVisible(CommonTitleBar::BTN_MAXIMUM, true);
	ui->top_widget->setBtnVisible(CommonTitleBar::BTN_MINIMUM, true);
	ui->top_widget->setBtnVisible(CommonTitleBar::BTN_RESTORE, true);

	ui->pushButtondata->setText(QString::fromLocal8Bit(tr("确定")));
	QHeaderView *headerView = ui->tableWidget->verticalHeader();
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setWindowTitle(QString::fromLocal8Bit(tr("数据显示")));
	
}

DatatableDialog::~DatatableDialog()
{
    delete ui;
}

void DatatableDialog::setTitile(QString name)
{
    ui->label->setText(name);
}

void DatatableDialog::setModal(TableModel &model)
{

	for (int row = 0; row < model.VerticalHeaderData().size(); row++)
	{
		int RowCont;
		RowCont = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(RowCont);//增加一行
		QString datav = model.m_rows[row][0].toString(); //data(model.index(row, 0));
		ui->tableWidget->setItem(row, 1, new QTableWidgetItem(datav));
		QString value = model.VerticalHeaderData().at(row);// ;[row][1].toString();//data(model.index(row, 1)).toString();
		QTableWidgetItem *item = new QTableWidgetItem(value);// new QTableWidgetItem
		ui->tableWidget->setItem(row, 0, item);
		
	}
}

void DatatableDialog::setselectrow(int row)
{
	ui->tableWidget->selectRow(row);
}

void DatatableDialog::on_pushButtondata_clicked()
{
	//ui->tableWidget->setRowCount(0);
	ui->tableWidget->clearContents();
	//ui->tableWidget->setRowCount(1);
    accept();
}
