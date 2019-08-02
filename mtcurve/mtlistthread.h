#ifndef MTLISTTHREAD_H
#define MTLISTTHREAD_H

#include <QThread>
#include <QMap>
#include "historyapk/hiscurve/hiscurve_access.h"
#include "historyapk/hiscurve/hiscurve_stru.h"
#include "rtdbblock/firdevblock/substationBaseReader.h"
#include "rtdbblock/firdevblock/bayBaseReader.h"
#include "rtdbblock/scadablock/rtanalogBaseReader.h"
#include "rtdbblock/firdevblock/tableiddefine_firdevblock.h"
#include "mtscadaobjlib/mtscadaobjlib_rtdbobject.h"


class mtListThread : public QThread
{
    Q_OBJECT
public:
	mtListThread(QWidget *parent = 0);
	QList<RTANALOG_DATA*> getAnalogList(){ return m_rtanalogList; }
	QMap<int, QString> getSubstation(){ return substationIdName; }
	QMultiMap<int, QMap<int, QString> > getGap(){ return gapIdName; }
protected:
	virtual void run();
 private:
	
	QMap<int,QString>substationIdName;
	QMultiMap<int,QMap<int, QString> > gapIdName;
	QList<RTANALOG_DATA*> m_rtanalogList;
};

#endif // MTLISTTHREAD_H
