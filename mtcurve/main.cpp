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
		QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("数据库初始化失败\n请通过工程管理器运行工程组态，启动集成开发环境。"),QString::fromLocal8Bit("确定"));
		return -1;
	}
		
    MainWindow w;
    w.show();

    return a.exec();
}
