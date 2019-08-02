#include "mainwindow.h"
#include "deviceapk/modelimportoollib/mit_modelRtdbMgr.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MITCModelDBMgr *pModelMgr = MITCModelDBMgr::createInstance(OPER_MODE_RDWR);
	if (NULL == pModelMgr)
	{
		QMessageBox::warning(NULL,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���ݿ��ʼ��ʧ��\n��ͨ�����̹��������й�����̬���������ɿ���������"),QString::fromLocal8Bit("ȷ��"));
		return -1;
	}
		
    MainWindow w;
    w.show();

    return a.exec();
}
