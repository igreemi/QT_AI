#ifndef WORKLOAD_STATISTICS_H
#define WORKLOAD_STATISTICS_H

#include <QDialog>
#include <QAbstractTableModel>
#include "qcustomplot.h"

namespace Ui {
class WorkloadStatistics;
}

class WorkloadStatistics : public QDialog
{
    Q_OBJECT

public:
    explicit WorkloadStatistics(QWidget *parent = nullptr);
    ~WorkloadStatistics();

    void closeEvent(QCloseEvent *event);
    void setAirportName(QString name);
   // void setMonthlyStatistics(QSqlQueryModel *monthlyStat);
   // void setYearStatistics(QSqlQueryModel *yearStat);

    void setMonthlyStatistics(QAbstractTableModel *monthlyStat);
    void setYearStatistics(QAbstractTableModel *yearStat);

    void setMonthComboBoxClear();
    void SendGraphickMonth(int month);
    void SendAnnualSchedule();

private:
    Ui::WorkloadStatistics *ui;
    QCPGraph *graph;
    QCPBars *bGraph;
    QAbstractTableModel *monthlyStatModel;
    QAbstractTableModel *yearStatModel;

signals:
    void sig_UnlockMainWindows(bool);

private slots:
    void on_cB_month_activated(int index);
};

#endif // WORKLOAD_STATISTICS_H
