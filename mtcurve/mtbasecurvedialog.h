#ifndef MTBASECURVEDIALOG_H
#define MTBASECURVEDIALOG_H

#include <QDialog>
#include "mtkdchart.h"
#include "qupdatethread.h"

namespace Ui {
class BaseCurveDialog;
}

class BaseCurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseCurveDialog(QWidget *parent = 0);
    ~BaseCurveDialog();

	void setlogIndex(int index);
	void setCurveNmae(QString name);

signals:
    void showTreeselect(bool isshow);
	void otherHide(bool ishsow);

private slots:
	void setCurrentLinesShow(bool isshow, int row);
	void setCurrentLinesColor();
    void getInterval(int index);
	bool painCurve(time_t start_date=0, time_t end_date=0);
	void slot_savePic(bool);
	void slot_print(bool);
	bool slot_treeChanged(int row);
	void slot_timeChange();
	void finishedThreadSlot();
	void slot_setcombox_2(int index);
	void slot_setcombox_3(int index);
	void settingTableChanged(int row, int col);
	void hideWidget();
	void showmax();
	void slot_hide(bool isdhow);
	void setcountdown();
	void setupdatetimer();
	void setinitialValue();
    void on_pushButton_slider_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_rangeChanged(int min, int max);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

protected:
	void timerEvent(QTimerEvent * t);

private:
    Ui::BaseCurveDialog *ui;
    mtListThread *m_fetchData;
	HisCurveAccess curve_access;
    mtKDchart *m_kdChart;
    ChartPaintThread *m_paintThread;
    int m_interval;
	int m_timegap;
	int setplength;
	int sliderinterval;
    int m_anaglogIndex;
    int m_row;
    QTimer *m_timer;
	QString m_curvename;
	QPrinter* m_pPrinter;
	QList<int>insertLineList;
	QUpdateThread *m_updateThread;
	int id1;
	int minute;
	int second;
	bool ismax;
};

#endif // MTBASECURVEDIALOG_H
