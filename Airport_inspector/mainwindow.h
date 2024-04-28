#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QAbstractTableModel>

#include "database.h"
#include "workload_statistics.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sig_SendRequestFlights(QString request, QString airportСode);

public slots:
    void ScreenDataFromDB(QAbstractTableModel *model);
    void ReceiveStatusConnectionToDB(bool status);
    void FillInTheListOfAirports(QAbstractTableModel *model);


private slots:
    void on_pB_statistics_clicked();

    void on_pB_workload_clicked();

private:
    Ui::MainWindow *ui;

    database* dataBase;
    QMessageBox* msg;
    bool statusConnection = false;
    QTimer timer;
    WorkloadStatistics *WLStat;

    QString getAirportCode(int row);
    QString getAirportName(int row);
};
#endif // MAINWINDOW_H
