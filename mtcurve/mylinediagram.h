#ifndef MYLINEDIAGRAM_H
#define MYLINEDIAGRAM_H

#include <QObject>
#include <kdchart/KDChartLineDiagram>
#include <kdchart/KDChartAbstractCoordinatePlane>
#include <kdchart/KDChartLeveyJenningsDiagram>
#include <QMouseEvent>
#include "datatabledialog.h"

class MyLineDiagram : public KDChart::LineDiagram
{

public:
	MyLineDiagram(QWidget* parent = 0, KDChart::CartesianCoordinatePlane* plane = 0);
	QColor getColor(){ return m_color; }
	time_t getstart(){ return start_time; }
	time_t getend(){ return end_time; }
	TableModel *getmodel(){ return m_model; }
	void setMymodel(TableModel *_model){ m_model = _model; }
	int getId(){ return m_id; }
	bool IsShow(){ return b_show; }
	bool IsShowValue(){ return b_showvalue; }
	bool IsShowMark(){ return b_showmark; }
    void setColor(QColor _color){ m_color = _color; }
	void setshow(bool _isshow);
	void setShowValue(bool _isshow);
	void setStartTime(time_t _start){ start_time = _start; }
	void setendtime(time_t _end){ end_time =_end; }
	void setShowMark(bool _isshow);
	void setId(int _id){ m_id = _id; }
private:
	QColor m_color;
	bool b_show;
	bool b_showvalue;
	bool b_showmark;
	int m_id;
	TableModel *m_model;
	time_t start_time;
	time_t end_time;
	QPointF pos;
protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MYLINEDIAGRAM_H