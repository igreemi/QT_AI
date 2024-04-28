#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lb_statusConnect->setText("Отключено");
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->rB_arrival->setChecked(true);

    dataBase = new database(this);
    msg = new QMessageBox();
    WLStat = new WorkloadStatistics();

    connect(dataBase, &database::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    connect(&timer, &QTimer::timeout, dataBase, &database::ConnectToDatabse);

    connect(dataBase, &database::sig_SendAirportsList, this, &MainWindow::FillInTheListOfAirports);

    connect(this, &MainWindow::sig_SendRequestFlights, dataBase, &database::RequestFlight);

    connect(dataBase, &database::sig_SendListOfAirports,this, &MainWindow::ScreenDataFromDB);

    connect(WLStat, &WorkloadStatistics::sig_UnlockMainWindows, this, &MainWindow::setEnabled);

    connect(dataBase, &database::sig_SendMonthlyStatistics, WLStat, &WorkloadStatistics::setMonthlyStatistics);

    connect(dataBase, &database::sig_SendYearStatistics, WLStat, &WorkloadStatistics::setYearStatistics);

    dataBase->AddDataBase();

    dataBase->ConnectToDatabse();

    timer.start(5000);
}

MainWindow::~MainWindow()
{
    delete WLStat;
    delete msg;
    delete ui;
}

void MainWindow::ScreenDataFromDB(QAbstractTableModel *model)
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    ui->tV_tableResult->setModel(proxyModel);

    QString date = ui->dE_dateEntry->date().toString(Qt::ISODate);

    proxyModel->setFilterRegularExpression(date);
    proxyModel->setFilterKeyColumn(1);

    ui->tV_tableResult->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tV_tableResult->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tV_tableResult->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    WLStat->setEnabled(status);

    if(WLStat->isVisible() || status == false || ui->cB_airport->count() == 0)
    {
        setEnabled(false);
    }
    else
    {
        setEnabled(true);
    }

    if(status)
    {
        ui->lb_statusConnect->setText("Подключено");
        ui->lb_statusConnect->setStyleSheet("color:green");
    }
    else
    {
        dataBase->DisconnectFromDataBase();
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");

        msg->exec();

        if(msg->isVisible())
        {
            timer.stop();
        }
        else
        {
            timer.start(5000);
        }
    }
}

void MainWindow::FillInTheListOfAirports(QAbstractTableModel *model)
{
    ui->cB_airport->setModel(model);
}

void MainWindow::on_pB_statistics_clicked()
{
    int row = ui->cB_airport->currentIndex();

    if(ui->rB_arrival->isChecked())
    {
        emit sig_SendRequestFlights("arrival", getAirportCode(row));
    }
    else if(ui->rB_departure->isChecked())
    {
        emit sig_SendRequestFlights("departure", getAirportCode(row));
    }
}


void MainWindow::on_pB_workload_clicked()
{
    int row = ui->cB_airport->currentIndex();

    dataBase->MonthlyStatistics(getAirportCode(row));
    dataBase->YearStatistics(getAirportCode(row));

    WLStat->SendGraphickMonth(0);
    WLStat->SendAnnualSchedule();
    WLStat->setAirportName(getAirportName(row));
    WLStat->setMonthComboBoxClear();
    WLStat->show();

    setEnabled(false);
}

QString MainWindow::getAirportCode(int row)
{
    QModelIndex airportCode = ui->cB_airport->model()->index(row, 1);
    return airportCode.data(Qt::DisplayRole).toString();
}

QString MainWindow::getAirportName(int row)
{
    QModelIndex airportName = ui->cB_airport->model()->index(row, 0);
    return airportName.data(Qt::DisplayRole).toString();
}

