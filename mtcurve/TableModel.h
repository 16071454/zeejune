#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QVariant>
#include <QVector>
#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QStringList>

struct CurvDataStruct
{
	QString startTime;
	QString endTime;
	int index;
	double _value;
};

class  TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel( QObject* parent = 0 );
    ~TableModel();

   
   QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const;

    int columnCount( const QModelIndex& parent = QModelIndex() ) const;

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	bool loadFromDataBase(QList<CurvDataStruct> _listdata, QString name);
	const QString titleText() const {
        return m_titleText;
    }

    void setTitleText( const QString& txt ) {
        m_titleText = txt;
    }

    /** Make the model invalid, that is, provide no data. */
    void clear();

    /**
     * Set to false if the data has no horizontal header
     */
    void setDataHasHorizontalHeaders( bool value ) {
        m_dataHasHorizontalHeaders = value;
    }
    /**
     * Set to false if the data has no vertical header
     */
    void setDataHasVerticalHeaders( bool value ) {
        m_dataHasVerticalHeaders = value;
    }
    /**
     * setSupplyHeaderData(false) allows to prevent the model from supplying header data,
     * even if parsing found any
     */
    void setSupplyHeaderData( bool value ) {
        m_supplyHeaderData = value;
    }
	QStringList VerticalHeaderData(){ return m_verticalHeaderData; }
	QVector< QVector<QVariant> > m_rows;
//protected:
    // the vector of rows:
	//QVector< QVector<QVariant> > m_rows;

private:

    // the header data:
    QStringList m_horizontalHeaderData;
    QStringList m_verticalHeaderData;
    QString m_titleText;
    bool m_dataHasHorizontalHeaders;
    bool m_dataHasVerticalHeaders;
    bool m_supplyHeaderData;
	
};


#endif
