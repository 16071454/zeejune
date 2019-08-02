#ifndef QUPDATETHREAD_H
#define QUPDATETHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "mtlistthread.h"
#include "scadaapk/scadaapi.hpp"
#include "chartpaintthread.h"
#include "hisdbaccess/hisdbaccess.h"

class mtKDchart;

class QUpdateThread : public QThread
{
	Q_OBJECT
public:
	QUpdateThread(mtKDchart *gui, ChartPaintThread *_paintThread);
	void setAnaglogIndex(int index);
	void setInterval(int interval);
	void settimegap(int timegap);
	void setcname(QString name);
	//void setcuracess(HisCurveAccess &cur);
signals:
	void setWaittimer();
	void setUpdatetimer();
	void setinitial();
protected:
     void run();
public slots:
    void slot_timeChange();
private:
    QTimer *m_timer;
    mtKDchart *m_kdChart;
    int m_anaglogIndex;
	int m_interval;
	int m_timegap;
	ChartPaintThread *m_paintThread;
	HisCurveAccess curve_access;
	QString cname;
};

#endif // QUPDATETHREAD_H
