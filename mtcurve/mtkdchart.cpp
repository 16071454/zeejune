#include "mtkdchart.h"
#include <QPainter>
#include <QListIterator>
#include <kdchart/KDChartFrameAttributes>
#include <kdchart/KDChartBackgroundAttributes>
#include <QMessageBox>
#include <QLine>
#include <QPair>

mtKDchart::mtKDchart(QWidget* parent) :
KDChart::Chart(parent),
xAxisline(NULL),
m_Linesx(NULL),
iscanpainter(true)
{
	m_curColumn = -1;
	m_curOpacity = 0;
	minsize = 0;
	m_changeXa = false;
	m_BarchangeXa = false;
	b_isLimit = false;
	isbar = false;
	setFocusPolicy(Qt::WheelFocus);
	setMouseTracking(true);
	m_plane = new KDChart::CartesianCoordinatePlane(this);
	m_Barplane = new KDChart::CartesianCoordinatePlane(this);
	
	path = new QPainterPath();
	path->addEllipse(-2.0, -1.0, 4.0, 2.0);
	m_lines = new MyLineDiagram (this);
	m_model = new TableModel(this);
	m_lines->setModel(m_model);

	m_plane->setReferenceCoordinatePlane(coordinatePlane());
	m_Barplane->setReferenceCoordinatePlane(coordinatePlane());
	setGlobalLeading(0, 0, 0, 0);
	
	addCoordinatePlane(m_plane);
	
	m_legend = new KDChart::Legend(this);
	m_legend->setTitleText(QString::fromLocal8Bit(tr("Í¼Àý")));
	m_legend->setPosition(KDChart::Position::North);
	m_legend->setAlignment(Qt::AlignCenter);
	m_legend->setOrientation(Qt::Horizontal);
	m_legend->setLegendStyle(KDChart::Legend::MarkersOnly);

	/*KDChart::TextAttributes lta(m_legend->textAttributes());
	lta.setPen(QPen(Qt::darkGray));
	KDChart::Measure me(5);
	me.setValue(me.value() * 0.7);
	lta.setFontSize(KDChart::Measure(2, KDChartEnums::MeasureCalculationModeAbsolute));
	m_legend->setTextAttributes(lta);*/
	addLegend(m_legend);
}

mtKDchart::~mtKDchart()
{
	delete path;
}

QPointF mtKDchart::findNewZoomCenter(const QPoint & pos)
{
	if (!height() || !width()) return m_plane->zoomCenter();

	const qreal coordWidth = 1.0;
	const qreal coordHeight = 1.0;
	const qreal resX = static_cast<qreal>(coordWidth / m_plane->zoomFactorX()) / width();
	const qreal resY = static_cast<qreal>(coordHeight / m_plane->zoomFactorY()) / height();
	//    qDebug() << "resX = " << resX << "  resY = " << resY;
	const qreal dX = (pos.x() - 0.5*width()) * resX;
	const qreal dY = (pos.y() - 0.5*height()) * resY;
	//    qDebug() << "dX = " << dX << "  dY = " << dY;
	const qreal zoomCenterX = m_plane->zoomCenter().x() + dX;
	const qreal zoomCenterY = m_plane->zoomCenter().y() + dY;
	return QPointF(zoomCenterX, zoomCenterY);
}



void mtKDchart::on_threeDModeCB_toggled(bool checked)
{
	KDChart::ThreeDLineAttributes td(m_lines->threeDLineAttributes());
	td.setDepth(3);
	if (checked)
		td.setEnabled(true);
	else
		td.setEnabled(false);

	m_lines->setThreeDLineAttributes(td);

	update();
}

void mtKDchart::on_paintThreeDBarsCB_toggled()
{
	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	KDChart::CartesianAxis* xBarAxisline = NULL;
	if (m_diagramlist.isEmpty())
	{
		setVisible(false);
	}

	if (!m_Barplane->isEmpty())
	{
		KDChart::AbstractDiagramList m_Bardiagramlist = m_Barplane->diagrams();
		QListIterator<KDChart::AbstractDiagram*> iter(m_Bardiagramlist);
		while (iter.hasNext())
		{
			KDChart::BarDiagram *tempBars = (KDChart::BarDiagram *)iter.next();
			m_Barplane->takeDiagram(tempBars);
		}
	}
	
	for (int i = 0; i < m_diagramlist.size(); i++)
	{
		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);
		KDChart::BarDiagram *m_bar = new KDChart::BarDiagram(this);
		m_bar->setModel(templines->getmodel());

		if (xBarAxisline == NULL)
		{
			xBarAxisline = new KDChart::CartesianAxis(m_bar);
		}

		KDChart::CartesianAxis* yBarAxisline = new KDChart::CartesianAxis(m_bar);
		xBarAxisline->setPosition(KDChart::CartesianAxis::Bottom);
		yBarAxisline->setPosition(KDChart::CartesianAxis::Left);

		m_bar->addAxis(xBarAxisline);
		m_bar->addAxis(yBarAxisline);
		KDChart::BarAttributes ba(m_bar->barAttributes());
		
		for (int row = 0; row < templines->getmodel()->columnCount(); ++row) 
		{
			m_bar->setBrush(row, QBrush(templines->getColor()));
			const QFont font(QFont("Comic", 10));
			KDChart::DataValueAttributes a(m_bar->dataValueAttributes(row));
			KDChart::TextAttributes ta(a.textAttributes());

			ta.setRotation(0);
			ta.setFont(font);
			ta.setPen(QPen(templines->getColor()));
			ta.setVisible(true);
			a.setTextAttributes(ta);
			a.setVisible(false);
			m_bar->setDataValueAttributes(row, a);
		}
		m_Barplane->addDiagram(m_bar); 
	}
	takeCoordinatePlane(m_plane);
	addCoordinatePlane(m_Barplane);
	update();
}

void mtKDchart::on_paintLinesCB_toggled(bool checked, QColor curcolor /*=Qt::green*/)
{
	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	if (m_diagramlist.isEmpty())
	{
		setVisible(false);
		return;
	}
	if (m_changeXa)
	{

		delete  xAxisline;
		xAxisline = NULL;

		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.first();
		xAxisline = new KDChart::CartesianAxis(templines);
		xAxisline->setPosition(KDChart::CartesianAxis::Bottom);
		m_Linesx = templines;
		m_changeXa = false;
		minsize = templines->getmodel()->rowCount();

	}
	 QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
	 while (iter.hasNext())
	 {
		 MyLineDiagram *templines = (MyLineDiagram *)iter.next();
		 templines->addAxis(xAxisline);

		 KDChart::DataValueAttributes dva(templines->dataValueAttributes());
		 KDChart::MarkerAttributes ma(dva.markerAttributes());
		 ma.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);
		 ma.setMarkerColor(templines->getColor());
		 dva.setMarkerAttributes(ma);
	 
		QPen linePen(templines->getColor());
		templines->setPen(0, linePen);
		templines->setDataValueAttributes(0, dva);
	   
	 }
	 update();
}


void mtKDchart::on_paintMarkersCB_toggled(MyLineDiagram *templines, int iColumn)
{
	
	KDChart::DataValueAttributes dva(templines->dataValueAttributes());
	KDChart::MarkerAttributes ma(dva.markerAttributes());
	ma.setMarkerSize(QSize(5, 5));
	ma.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);
	ma.setVisible(true);
	ma.setMarkerColor(templines->getColor());

	dva.setMarkerAttributes(ma);	
	dva.setVisible(false);
	templines->setDataValueAttributes(iColumn, dva);
	update();
}

void mtKDchart::on_paintValuesCB_toggled()
{
	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	if (m_diagramlist.isEmpty())
	{
		setVisible(false);
		return;
	}
	for (int i = 0; i < m_diagramlist.size(); i++)
	{
		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);

		const int colCount = templines->model()->rowCount(templines->rootIndex());
		for (int iColumn = 0; iColumn < colCount; ++iColumn) {
			KDChart::DataValueAttributes a(templines->dataValueAttributes(iColumn));
			QBrush brush(templines->getColor());
			KDChart::TextAttributes ta(a.textAttributes());
			ta.setRotation(0);
			ta.setFont(QFont("Comic"));
			ta.setPen(QPen(brush.color()));
			ta.setVisible(templines->IsShowValue());
			ta.setFontSize(20);
			a.setVisible(true);
			a.setTextAttributes(ta);
			templines->setDataValueAttributes(iColumn, a);
		}
	}
	update();
}


void mtKDchart::addLineDiagram(QList<CurvDataStruct> &_listdata, QString name, int id, time_t start_time, time_t end_time)
{
	
	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	QList<int>idlist;
	
	for (int i = 0; i < m_diagramlist.size(); i++)
	{
		MyLineDiagram *templines = (MyLineDiagram *)m_diagramlist.at(i);
		idlist.append(templines->getId());
		if ((templines->getId() == id && templines->getstart() != start_time) || (templines->getId() == id &&templines->getend() != end_time))
		{
			MyLineDiagram *m_lines2 = new MyLineDiagram(this);
			TableModel *m_model2 = new TableModel(this);
			m_model2->loadFromDataBase(_listdata, name);
			m_lines2->setModel(m_model2);
			m_lines2->setMymodel(m_model2);

			m_lines2->setColor(templines->getColor());
			m_lines2->setshow(templines->IsShow());
			m_lines2->setStartTime(start_time);
			m_lines2->setendtime(end_time);

			m_lines2->setObjectName(name);
			m_lines2->setId(templines->getId());

			KDChart::CartesianAxis* yAxisline = new KDChart::CartesianAxis(m_lines2);
			xAxisline->setPosition(KDChart::CartesianAxis::Bottom);
			yAxisline->setPosition(KDChart::CartesianAxis::Right);

			m_lines2->addAxis(xAxisline);
			m_lines2->addAxis(yAxisline);

			m_plane->replaceDiagram(m_lines2, templines);
			m_legend->replaceDiagram(m_lines2, templines);
			m_legend->adjustSize();
			m_legend->update();

		}
		else
		{
			continue;
		}
	}
	
	if (idlist.contains(id))
	{
		return;
	}
	TableModel *m_model1 = new TableModel(this);
	MyLineDiagram *m_lines1 = new MyLineDiagram(this);
	m_lines1->setColor(Qt::green);
	m_lines1->setshow(true);
	m_lines1->setShowValue(false);
	m_lines1->setShowMark(false);
	m_lines1->setId(id);
	m_lines1->setModel(m_model1);
	m_lines1->setMymodel(m_model1);
	m_lines1->setStartTime(start_time);
	m_lines1->setendtime(end_time);
	m_lines1->setObjectName(name);
	m_model1->loadFromDataBase(_listdata,name);
	
	if (m_diagramlist.isEmpty())
	{
		xAxisline = NULL;
	}
	if (xAxisline == NULL)
	{
		xAxisline = new KDChart::CartesianAxis(m_lines1);
		xAxisline->setPosition(KDChart::CartesianAxis::Bottom);
		minsize = m_model1->rowCount();
		m_Linesx = m_lines1;
	}
	//if (minsize < m_model1->rowCount())
	//{
		//xAxisline->setLabels(xAxlist);
	//}
	KDChart::CartesianAxis* xAxislinetemp = new KDChart::CartesianAxis(m_lines1);
	KDChart::CartesianAxis* yAxisline = new KDChart::CartesianAxis(m_lines1);

	yAxisline->setPosition(KDChart::CartesianAxis::Right);

	m_lines1->addAxis(xAxisline);
	m_lines1->addAxis(yAxisline);
	
	m_lines->setObjectName(name);

	m_plane->addDiagram(m_lines1);
	m_legend->addDiagram(m_lines1);
	//m_legend->adjustSize();
	//m_legend->update();
	update();
}

//void mtKDchart::setInitParmeter()
//{
//	m_linesConfigDlg = new LinesConfigDialog(this);
//	m_showMarkVector = m_linesConfigDlg->getlineMarkVector();
//	if (m_linesConfigDlg != NULL)
//	{
//		m_linesConfigDlg->setInit(m_plane);	
//	}
//	
//}

void mtKDchart::setLines()
{
	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
	while (iter.hasNext())
	{
		MyLineDiagram *dtemplines = (MyLineDiagram *)iter.next();
		if (dtemplines ->IsShow()== false)
		{
			m_plane->takeDiagram(dtemplines);
			
			m_legend->removeDiagram(dtemplines);
			if (m_Linesx->getId() == dtemplines->getId())
			{
				m_changeXa = true;
			}
			 delete dtemplines;
		}
	}

	//on_paintLinesCB_toggled(true);
	//on_paintValuesCB_toggled();
	//on_paintMarkersCB_toggled();
}

//void mtKDchart::setBars()
//{
//	KDChart::AbstractDiagramList m_diagramlist = m_Barplane->diagrams();
//	QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);
//	while (iter.hasNext())
//	{
//		MyBarDiagram *dtemplines = (MyBarDiagram *)iter.next();
//		if (dtemplines->IsShow() == false)
//		{
//			m_Barplane->takeDiagram(dtemplines);
//			m_legend->removeDiagram(dtemplines);
//			if (m_Barsx->getId() == dtemplines->getId())
//			{
//				m_BarchangeXa = true;
//			}
//
//		}
//	}
//	on_paintThreeDBarsCB_toggled();
//}

void mtKDchart::addBarDiagram()
{
	on_paintThreeDBarsCB_toggled();
}

void mtKDchart::changeLineToBars()
{
	takeCoordinatePlane(m_plane);
	addCoordinatePlane(m_Barplane);
}

void mtKDchart::changeBarsToLine()
{
	takeCoordinatePlane(m_Barplane);
    addCoordinatePlane(m_plane);
}

void mtKDchart::mouseMoveEvent(QMouseEvent* event)
{
	pos = event->pos();
	mousePos = m_plane->translateBack(pos);
	
	if (!isbar)
	{
		KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
		QListIterator<KDChart::AbstractDiagram*> iter(m_diagramlist);	
		moveList.clear();
		movenameList.clear();
		movePointList.clear();
		movecolorList.clear();
		int xpos = -1;
		
		while (iter.hasNext())
		{
			MyLineDiagram *templines = (MyLineDiagram *)iter.next();
			const QPair<QPointF,QPointF>scalePointF =templines->dataBoundaries();
			QPointF  _end= scalePointF.first;
			QPointF _start = scalePointF.second;

			int ylevel = qAbs(_start.y() - _end.y())*5;
			if (ylevel<=100)
			{
				ylevel = 200;
			}
			xlevel = qAbs(_start.x() - _end.x());
			qreal spaceinterval = qAbs(_end.y() - _start.y()) / ylevel;
			TableModel *_model = templines->getmodel();
			QVariant varX(mousePos.x());
			int xpos = varX.toInt();
			if (xpos<0)
			{
				continue;
			}
			for (int i = 0; i <= ylevel; i++)
			{
				//QPointF point(pos.x(), startorigin.y() + spaceinterval*i);
				pos = m_plane->translate(QPointF(xpos, _end.y()+spaceinterval*i));
				QModelIndex index = templines->indexAt(pos.toPoint());

				if (!index.isValid())
				{
					continue;
				}
				if (xpos< _model->rowCount())
					{
					
					QString _movestring = QString("%1:%2-%3").arg(templines->objectName()).arg(templines->getmodel()->headerData(xpos, Qt::Vertical).toString()).arg(templines->getmodel()->data(_model->index(xpos, 0)).toDouble());
					if (moveList.contains(_movestring) || movenameList.contains(templines->objectName()))
					{
						break;
					}
					movenameList.append(templines->objectName());
					moveList.append(_movestring);
					//if (!templines->IsShowMark())
					//	{
							QColor _color = templines->getColor();
							movePointList.append(pos);
							movecolorList.append(_color);
						//}
					}
				break;
			}
		}
	}
}

void mtKDchart::paintEvent(QPaintEvent * event)
{
	KDChart::Chart::paintEvent(event);
    if (!isVisible() || isbar ==true /*|| iscanpainter==false*/)
	{
		return;
	}

	KDChart::AbstractDiagramList m_diagramlist = m_plane->diagrams();
	if (m_diagramlist.isEmpty())
	{
		return;
	}
	if (mousePos.x() >= 0)
	{
		QPainter painter(this);
		painter.setPen(QPen(Qt::red, 1));
		QLineF lineF;
		QRect _rect;

		lineF.setLine(pos.x(), height() - 30, pos.x(), m_legend->height() + 20);
		painter.drawLine(lineF);
		int lenth = width() - pos.x();
		if (lenth < width() / 2)
		{
			_rect.setTopLeft(QPoint(pos.x() - (width() / 2.5), height() / 2));
			//_rect.setTopRight(QPoint(pos.x()-20,height()/2));
			_rect.setHeight(25 * moveList.size());
			_rect.setWidth((width() / 2));
		}
		else
		{
			_rect.setTopLeft(QPoint(pos.x() + 20, height() / 2));
			_rect.setHeight(25 * moveList.size());
			_rect.setWidth(((width() / 2)));
		}
		//if (moveList.size() <= m_diagramlist.size())
		//{
		QString value;
		for (int i = 0; i < moveList.size(); i++)
		{
			value.append(moveList.at(i));
			value.append("\n");
		}
		QFont font1("ËÎÌå", 12, QFont::Bold, true);
		painter.setFont(font1);
		//painter.setPen(QPen(Qt::blue, 1));

		painter.drawText(_rect, Qt::AlignAbsolute, value);
		for (int i = 0; i < movePointList.size(); i++)
		{
			painter.setBrush(movecolorList.at(i));
			painter.drawEllipse(movePointList.at(i), 5, 5);
		}
	}
	update();
}

void mtKDchart::setcanpaint(bool iscan)
{
	iscanpainter = iscan;
}


