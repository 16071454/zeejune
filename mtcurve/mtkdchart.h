#ifndef MTKDCHART_H
#define MTKDCHART_H
#include <QObject>
#include <kdchart/KDChartChart>
#include <kdchart/KDChartCartesianCoordinatePlane>
#include <kdchart/KDChartLeveyJenningsCoordinatePlane>
#include <kdchart/KDChartCartesianAxis>
#include <kdchart/KDChartLineDiagram>
#include <kdchart/KDChartPlotter>
#include <kdchart/KDChartTextAttributes>
#include <kdchart/KDChartDataValueAttributes>
#include <kdchart/KDChartThreeDLineAttributes>
#include <kdchart/KDChartGridAttributes>
#include <kdchart/KDChartAbstractCoordinatePlane>
#include <kdchart/KDChartLegend>
#include <kdchart/KDChartBarDiagram>
#include <kdchart/KDChartLeveyJenningsAxis>
#include <kdchart/KDChartThreeDBarAttributes>
#include <QList>
#include <QString>
#include <QPoint>
#include <QStringList>
#include <QPen>
#include <QFont>
#include <QColor>
#include <QPolygonF>
#include <QPainterPath>
#include <QDateTime>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMultiMap>
#include "mylinediagram.h"
//#include "TableModel.h"


class mtKDchart : public KDChart::Chart
{
	Q_OBJECT
public:
	explicit mtKDchart(QWidget* parent=0);
	~mtKDchart();
	
	void setchartDataList(QList<CurvDataStruct>_chartDatalist){ m_chartDatalist = _chartDatalist; }
	QList<CurvDataStruct>& getchartDataList(){ return m_chartDatalist; }
	MyLineDiagram* getlines(){ return m_lines; }
	TableModel * getmodel(){ return m_model; }
	QColor getcolor(){ return m_curColor; }
	void setBlimit(bool isshow){ b_isLimit = isshow; }
	KDChart::CartesianCoordinatePlane *getCartesianPlane(){ return m_plane; }
	
	void addBarDiagram();
	QMap<int, QString>& getlinesMap(){ return m_selectIDvector; }
	//void setInitParmeter();
	void setLines();
	//void setBars();
	void setIsbar(bool _isbar){ isbar = _isbar; }
	//KDChart::Legend * getLengent(){ return m_legend; }
	QVector<bool>* getlineMarkVector(){ return m_showMarkVector; }
	void changeLineToBars();
	void changeBarsToLine();
	void setcanpaint(bool iscan);
	void addLineDiagram(QList<CurvDataStruct> &_listdata, QString name, int id, time_t start_time, time_t end_time);
protected:
	QPointF findNewZoomCenter(const QPoint & pos);
	
public slots:
		void on_paintMarkersCB_toggled(MyLineDiagram *templines, int iColumn);
		void on_paintLinesCB_toggled(bool checked,QColor curcolor = Qt::green);
		void on_paintValuesCB_toggled();
		void on_threeDModeCB_toggled(bool checked);
		void on_paintThreeDBarsCB_toggled();
		
private:
//	void paintEvent(QPaintEvent* event);
	QList<CurvDataStruct>m_chartDatalist;
	KDChart::Legend *m_legend;
	KDChart::CartesianCoordinatePlane *m_plane;
	KDChart::CartesianCoordinatePlane *m_Barplane;
	MyLineDiagram* m_lines;
	TableModel *m_model;
	QPointF mousePos;
	int xlevel;
	int m_curRow;
	int m_curColumn;
	int m_curOpacity;
	QPainterPath* path;
	QColor m_curColor;
	bool b_isLimit;
	int minsize;
	KDChart::CartesianAxis* xAxisline;
	
	QMap<int, QString> m_selectIDvector;

	MyLineDiagram* m_Linesx;
	QVector<bool>*m_showMarkVector;
	bool m_changeXa;
	bool m_BarchangeXa;
	bool isbar;
	QPointF pos;
	QList<QString>moveList;
	QList<QString>movenameList;
	QList<QPointF>movePointList;
	QList<QColor>movecolorList;
	QPointF m_rectPoint;
	bool iscanpainter;
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent * event);

};


#endif // MTKDCHART_H
