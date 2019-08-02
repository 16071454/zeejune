#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QObject>
#include <kdchart/KDChartWidget>
#include <kdchart/KDChart>
#include <kdchart/KDChartAbstractCoordinatePlane>
#include <kdchart/KDChartLegend>
#include <kdchart/KDChartCartesianCoordinatePlane>
#include <QKeyEvent>

class curveWidget : public KDChart::Chart
{
     Q_OBJECT
public:
	explicit curveWidget(QWidget* parent = 0);
	KDChart::CartesianCoordinatePlane *getCartesianPlane(){ return m_plane; }
protected:
	QPointF findNewZoomCenter(const QPoint & pos);
	void mousePressEvent(QMouseEvent * e);
	void wheelEvent(QWheelEvent* e);
	void keyPressEvent(QKeyEvent* e);
private:
	KDChart::Legend *m_legend;
	KDChart::CartesianCoordinatePlane *m_plane;
};

#endif // CURVEWIDGET_H
