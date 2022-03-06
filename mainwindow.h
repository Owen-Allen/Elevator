#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "carmodel.h"
#include "passenger.h"
#include "timer.h"

#include <iostream>
using namespace std;
#include <list>

#include <QDebug>


const int NUM_CARS = 2;
const int NUM_PASSENGERS = 5;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CarModel *cars[NUM_CARS];
    Passenger *people[NUM_PASSENGERS];

    list<Passenger*> pickup_requests;

    QTimer *control_clock;
    bool determine_elevator(Passenger*);
    void create_pickup_request(); // not in use?


private:
    Ui::MainWindow *ui;

signals:
    void request_upcoming_passenger(Passenger*); // not in use

    void send_overload();
    void send_fire();
    void send_powerout();
    void send_doorblocked();
    void send_help();


private slots:
    void overload_sensor();
    void fire_sensor();
    void power_sensor();
    void door_sensor();
    void help();
    void normal();
//    void add_request(Passenger*);

};
#endif // MAINWINDOW_H
