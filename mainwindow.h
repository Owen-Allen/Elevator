#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "carmodel.h"
#include "passenger.h"

#include <iostream>
using namespace std;
#include <queue>


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

    queue<Passenger*> pickup_requests;

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




};
#endif // MAINWINDOW_H
