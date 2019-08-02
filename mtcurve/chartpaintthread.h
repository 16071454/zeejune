#ifndef CHARTPAINTTHREAD_H
#define CHARTPAINTTHREAD_H

#include <QObject>
#include <QThread>
#include "TableModel.h"
class mtKDchart;

class ChartPaintThread : public QThread
{
        Q_OBJECT
public:
	ChartPaintThread(mtKDchart *gui);
	mtKDchart * getchart(){ return m_kdchart; }
	void setrun(bool _isrun) { isrun = _isrun; }
signals:
	void emitaddLineDiagram();
public slots:
   void slotaddLineDiagram();
protected:
    virtual void run();
private:
    mtKDchart *m_kdchart;
	bool isrun;
};

#endif // CHARTPAINTTHREAD_H
