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
const int NUM_PASSENGERS = 3;


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

    Timer *control_clock;
    bool determine_elevator(Passenger*);
    void create_pickup_request();

private:
    Ui::MainWindow *ui;

private slots:
    void overload_sensor();
    void fire_sensor();
    void power_sensor();
    void door_sensor();
    void help();
    void normal();
    void add_request(Passenger*);

signals:
    void request_upcoming_passenger(Passenger*);

};
#endif // MAINWINDOW_H
