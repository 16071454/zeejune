#include "curvewidget.h"

curveWidget::curveWidget(QWidget *parent):
KDChart::Chart(parent),
m_plane(NULL)
{
    setFocusPolicy( Qt::WheelFocus );
	m_plane = new KDChart::CartesianCoordinatePlane(this);
	addCoordinatePlane(m_plane);
}

QPointF curveWidget::findNewZoomCenter( const QPoint & pos )
{
	if (!height() || !width()) return m_plane->zoomCenter();

    const qreal coordWidth = 1.0;
    const qreal coordHeight = 1.0;

//    qDebug() << "pos = " << pos;
	const qreal resX = static_cast<qreal>(coordWidth / m_plane->zoomFactorX()) / width();
	const qreal resY = static_cast<qreal>(coordHeight / m_plane->zoomFactorY()) / height();
//    qDebug() << "resX = " << resX << "  resY = " << resY;
    const qreal dX = (pos.x() - 0.5*width() ) * resX;
    const qreal dY = (pos.y() - 0.5*height()) * resY;
//    qDebug() << "dX = " << dX << "  dY = " << dY;
	const qreal zoomCenterX = m_plane->zoomCenter().x() + dX;
	const qreal zoomCenterY = m_plane->zoomCenter().y() + dY;
    return QPointF( zoomCenterX, zoomCenterY );
}


void curveWidget::mousePressEvent( QMouseEvent * e )
{
//    const QPointF zoomCenter( findNewZoomCenter( e->pos() ) );
//	if (zoomCenter != m_plane->zoomCenter()) {
////        qDebug() << "zoom center = " << zoomCenter;
//		m_plane->setZoomCenter(zoomCenter);
//        update();
//    }
}


void curveWidget::wheelEvent( QWheelEvent* e )
{
    qreal delta = static_cast<qreal>( e->delta() ) / 120.0 / 10.0;
    m_plane->setZoomFactorX(m_plane->zoomFactorX() + delta);
    m_plane->setZoomFactorY(m_plane->zoomFactorY() + delta);
    update();
}

void curveWidget::keyPressEvent( QKeyEvent* e )
{
    int dZoom = 0;
    qreal dX = 0;
    qreal dY = 0;
    switch ( e->key() ) {
        case Qt::Key_PageDown:
            dZoom = 1;
            break;
        case Qt::Key_Down:
            dY = 0.1;
            break;
        case Qt::Key_Right:
            dX = 0.1;
            break;
        case Qt::Key_PageUp:
            dZoom = -1;
            break;
        case Qt::Key_Up:
            dY = -0.1;
            break;
        case Qt::Key_Left:
            dX = -0.1;
            break;
        default:
            e->ignore();
    }
    if ( dZoom || dX || dY ) {
		const double factor = qMax(double(1.0), m_plane->zoomFactorX() + dZoom);
		const qreal x = m_plane->zoomCenter().x() + dX;
		const qreal y = m_plane->zoomCenter().y() + dY;
		m_plane->setZoomFactorX(factor);
		m_plane->setZoomFactorY(factor);
		m_plane->setZoomCenter(QPointF(x, y));
        update();
    }
}
