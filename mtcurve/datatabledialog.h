#ifndef DATATABLEDIALOG_H
#define DATATABLEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "TableModel.h"

namespace Ui {
class DatatableDialog;
}

class DatatableDialog : public QDialog
{
    Q_OBJECT

public:
	explicit DatatableDialog( QWidget *parent = 0);
    ~DatatableDialog();
    void setTitile(QString name);
	void setModal(TableModel &model);
	void setselectrow(int row);

private slots:
void on_pushButtondata_clicked();

private:
    Ui::DatatableDialog *ui;
};

#endif // DATATABLEDIALOG_H
