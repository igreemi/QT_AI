#include "database.h"
#include "qdatetime.h"



database::database(QObject *parent)
    : QObject{parent}
{

    listOfAirports = new QSqlQueryModel(this);
    flightTable = new QSqlQueryModel(this);
    monthlyStat = new QSqlQueryModel(this);
    yearStat = new QSqlQueryModel(this);
    dataBase = new QSqlDatabase();
    tableView = new QTableView();

}

database::~database()
{
    delete dataBase;
}

void database::AddDataBase()
{
    *dataBase = QSqlDatabase::addDatabase(POSTGRE_DRIVER);
}

QSqlError database::GetLastError()
{
    return dataBase->lastError();
}

void database::ConnectToDatabse()
{
    dataBase->setHostName("981757-ca08998.tmweb.ru");
    dataBase->setDatabaseName("demo");
    dataBase->setUserName("netology_usr_cpp");
    dataBase->setPassword("CppNeto3");
    dataBase->setPort(5432);

    statusConnectDb = dataBase->open();

    if(updateAirportList != statusConnectDb)
    {
        if(dataBase->isOpen())
        {
            AirportList();
        }
        updateAirportList = statusConnectDb;
    }

    emit sig_SendStatusConnection(statusConnectDb);
}

void database::DisconnectFromDataBase()
{
    if(dataBase->isOpen())
    {
        dataBase->close();
    }
}

void database::AirportList()
{
    listOfAirports->setQuery("SELECT airport_name->>'ru' as airportName"
                    ", airport_code FROM bookings.airports_data", *dataBase);

       emit sig_SendAirportsList(listOfAirports);
}

void database::RequestFlight(QString request, QString airportСode)
{
    QString tmpRequest;
    QString tmpDirection;
    QString tmpAppointment;

    if(request == "arrival")
    {
        tmpRequest = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name from bookings.flights f "
                     "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                     "where f.arrival_airport  = '" + airportСode + "'";
        tmpDirection = "Время прилета";
        tmpAppointment = "Аэропорт отправления";

    }
    else     if(request == "departure")
    {
        tmpRequest = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name from bookings.flights f "
                     "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                     "where f.departure_airport  = '" + airportСode + "'";
        tmpDirection = "Время вылета";
        tmpAppointment = "Аэропорт прибытия";

    }

    flightTable->setQuery(tmpRequest, *dataBase);
    flightTable->setHeaderData(0, Qt::Horizontal, "Номер рейса");
    flightTable->setHeaderData(1, Qt::Horizontal, tmpDirection);
    flightTable->setHeaderData(2, Qt::Horizontal, tmpAppointment);

    emit sig_SendListOfAirports(flightTable);
}

void database::MonthlyStatistics(QString airportCode)
{

    QString request = "SELECT count(flight_no), date_trunc('day', scheduled_departure) "
                      "as Day from bookings.flights f "
                      "WHERE(scheduled_departure::date > date('2016-08-31') and "
                      "scheduled_departure::date <= date('2017-08-31')) and "
                      "( departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Day";

    monthlyStat->setQuery(request, *dataBase);

    emit sig_SendMonthlyStatistics(monthlyStat);
}

void database::YearStatistics(QString airportCode)
{
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as Month "
                      "from bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') and "
                      "scheduled_departure::date <= date('2017-08-31')) and "
                      "( departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "' ) "
                      "GROUP BY Month";


    yearStat->setQuery(request, *dataBase);

    emit sig_SendYearStatistics(yearStat);
}

