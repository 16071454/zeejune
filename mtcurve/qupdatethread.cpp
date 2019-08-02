#include <kdchart/KDChartBackgroundAttributes>
#include "qupdatethread.h"
#include "mtkdchart.h"



QUpdateThread::QUpdateThread(mtKDchart *gui, ChartPaintThread *_paintThread)
{
    m_kdChart = gui;
    m_anaglogIndex =-1;
	m_paintThread = _paintThread;
	m_timegap = 12 * 60 * 60;
}

void QUpdateThread::run()
{
	m_timer = new QTimer;
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeChange()));
	m_timer->start(15*60*1000);
	emit setWaittimer();
	exec();
}

void QUpdateThread::setAnaglogIndex(int index)
{
    m_anaglogIndex =index;
}

void QUpdateThread::setInterval(int interval)
{
	m_interval = interval;
}

void QUpdateThread::settimegap(int timegap)
{
	m_timegap = timegap;
}


void QUpdateThread::setcname(QString name)
{
	cname = name;
}

//void QUpdateThread::setcuracess(HisCurveAccess &cur)
//{
//	//curve_access = cur;
//}

void QUpdateThread::slot_timeChange()
{
    if (m_anaglogIndex == -1)
    {
        return;
    }

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

		
		time_t end_date = QDateTime::currentDateTime().toTime_t();
		time_t start_date = end_date - m_timegap;
        vector<Poco::SharedPtr<Poco::Data::RecordSet> > vec_results;
		int indexid = templines->getId();

		if (curve_access.init() == false)
		{
			cout << __LINE__ << " error!" << endl;
			return ;
		}
        if (!curve_access.query(HISCURVEACCESS::HDR_ANALOGINPUT,indexid , start_date, end_date, 1, vec_results))
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
			emit setUpdatetimer();
            m_kdChart->setVisible(true);
            m_kdChart->addLineDiagram(chartData, cname, m_anaglogIndex, start_date, end_date);
            m_paintThread->setrun(true);

        }
        else
        {
            //QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有数据"), QMessageBox::Ok);
            //m_kdChart->hide();
			emit setinitial();
            continue;

        }
    }
}
