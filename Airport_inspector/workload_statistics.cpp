#include "workload_statistics.h"
#include "ui_workload_statistics.h"



WorkloadStatistics::WorkloadStatistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkloadStatistics)
{
    ui->setupUi(this);

    QStringList monthList = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль",
                             "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    ui->cB_month->addItems(monthList);

}

WorkloadStatistics::~WorkloadStatistics()
{
    delete ui;
}

void WorkloadStatistics::closeEvent(QCloseEvent *event)
{
    if(this->close())
    {
        emit sig_UnlockMainWindows(true);
    }
}

void WorkloadStatistics::setAirportName(QString name)
{
    ui->lb_airportName->setText(name);
}

void WorkloadStatistics::setMonthlyStatistics(QAbstractTableModel *monthlyStat)
{
    monthlyStatModel = monthlyStat;
}

void WorkloadStatistics::setYearStatistics(QAbstractTableModel *yearStat)
{
    yearStatModel = yearStat;
}

void WorkloadStatistics::setMonthComboBoxClear()
{
    ui->cB_month->setCurrentIndex(0);
}

void WorkloadStatistics::SendGraphickMonth(int month)
{
    QVector<int> workload;
    QVector<QDate> workDate;

    for(int i = 0; i < monthlyStatModel->rowCount(); i++)
    {
        if(monthlyStatModel->data(monthlyStatModel->index(i, 1)).toDate().month() == month + 1)
        {
        workload.push_back(monthlyStatModel->data(monthlyStatModel->index(i, 0)).toInt());
        workDate.push_back(monthlyStatModel->data(monthlyStatModel->index(i, 1)).toDate());
        }
    }

    ui->wd_monthly_statistics->clearGraphs();

    graph = new QCPGraph(ui->wd_monthly_statistics->xAxis, ui->wd_monthly_statistics->yAxis);

    ui->wd_monthly_statistics->addGraph();

    for(int i = 0; i < workDate.size(); i++)
    {
        graph->addData(workDate[i].day(), workload[i]);
    }

    ui->wd_monthly_statistics->rescaleAxes();

    ui->wd_monthly_statistics->replot();
}

void WorkloadStatistics::SendAnnualSchedule()
{
    QVector<double> workload;
    QVector<QDate> workDate;
    QVector<QString> labels;

    for(int i = 0; i < yearStatModel->rowCount(); i++)
    {

        workload.push_back(yearStatModel->data(yearStatModel->index(i, 0)).toInt());
        workDate.push_back(yearStatModel->data(yearStatModel->index(i, 1)).toDate());

        int year = yearStatModel->data(yearStatModel->index(i, 1)).toDate().year();
        int month = yearStatModel->data(yearStatModel->index(i, 1)).toDate().month();

        QString monthYear;
        monthYear = QString::number(month) + "-" + QString::number(year);
        labels.push_back(monthYear);
    }

    ui->wd_statistics_for_the_year->clearPlottables();

    bGraph = new QCPBars(ui->wd_statistics_for_the_year->xAxis, ui->wd_statistics_for_the_year->yAxis);

    bGraph->setPen(QPen(QColor(0, 0, 0).lighter(170)));
    bGraph->setBrush(QColor(100, 120, 176));

    QVector<double> ticks;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);

    ui->wd_statistics_for_the_year->xAxis->setTicker(textTicker);
    ui->wd_statistics_for_the_year->xAxis->setTickLabelRotation(90);
    ui->wd_statistics_for_the_year->xAxis->setSubTicks(false);
    ui->wd_statistics_for_the_year->xAxis->setBasePen(QPen(Qt::black));
    ui->wd_statistics_for_the_year->xAxis->setTickPen(QPen(Qt::black));
    ui->wd_statistics_for_the_year->xAxis->grid()->setVisible(true);
    ui->wd_statistics_for_the_year->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->wd_statistics_for_the_year->xAxis->setTickLabelColor(Qt::black);
    ui->wd_statistics_for_the_year->xAxis->setLabelColor(Qt::black);

    ui->wd_statistics_for_the_year->yAxis->setRange(0, 1);
    ui->wd_statistics_for_the_year->yAxis->setPadding(5);
    ui->wd_statistics_for_the_year->yAxis->setBasePen(QPen(Qt::black));
    ui->wd_statistics_for_the_year->yAxis->setTickPen(QPen(Qt::black));
    ui->wd_statistics_for_the_year->yAxis->setSubTickPen(QPen(Qt::black));
    ui->wd_statistics_for_the_year->yAxis->grid()->setSubGridVisible(true);
    ui->wd_statistics_for_the_year->yAxis->setTickLabelColor(Qt::black);
    ui->wd_statistics_for_the_year->yAxis->setLabelColor(Qt::black);
    ui->wd_statistics_for_the_year->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->wd_statistics_for_the_year->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    for(int i = 0; i < workDate.size(); i++)
    {
        bGraph->addData(workDate[i].month(), workload[i]);
    }

    ui->wd_statistics_for_the_year->rescaleAxes();

    ui->wd_statistics_for_the_year->replot();
}



void WorkloadStatistics::on_cB_month_activated(int index)
{
    SendGraphickMonth(index);
}

