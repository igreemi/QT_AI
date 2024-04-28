#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSortFilterProxyModel>

#include <QTimer>

#define POSTGRE_DRIVER "QPSQL"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};


class database : public QObject
{
    Q_OBJECT

public:
    explicit database(QObject *parent = nullptr);
    ~database();
    void AddDataBase();
    QSqlError GetLastError(void);
    void ConnectToDatabse();
    void DisconnectFromDataBase();
    void AirportList();
    void RequestFlight(QString request, QString airportСode);
    void MonthlyStatistics(QString airportCode);
    void YearStatistics(QString airportCode);

private:
    QSqlDatabase *dataBase;
    QSqlQueryModel *listOfAirports;
    QSqlQueryModel *flightTable;
    QSqlQueryModel *monthlyStat;
    QSqlQueryModel *yearStat;
    QTableView *tableView;
    bool statusConnectDb = false;
    bool updateAirportList = false;



signals:
    void sig_SendStatusConnection(bool);
    void sig_SendAirportsList(QSqlQueryModel*);
    void sig_SendListOfAirports(QSqlQueryModel*);
    void sig_SendMonthlyStatistics(QSqlQueryModel*);
    void sig_SendYearStatistics(QSqlQueryModel*);


};

#endif // DATABASE_H
