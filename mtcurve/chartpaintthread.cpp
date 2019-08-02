#include "chartpaintthread.h"
#include "mtkdchart.h"

ChartPaintThread::ChartPaintThread(mtKDchart *gui)
{
	m_kdchart = gui;
	isrun = false;
	connect(this, SIGNAL(emitaddLineDiagram()), this, SLOT(slotaddLineDiagram()));
}

void ChartPaintThread::slotaddLineDiagram()
{
	m_kdchart->on_paintLinesCB_toggled(true);
	//m_kdchart->on_paintMarkersCB_toggled();
}

void ChartPaintThread::run()
{
	
	while (1)
	{
		if (isrun == false)
		{
			continue;
		}
		emit emitaddLineDiagram();
		isrun = false;
		msleep(100);
		
	}
}
