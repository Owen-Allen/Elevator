# 3004-a2


Owen Allen
101119431

3 main header files and cpp files

    Passenger.h Passenger.cpp
    CarModel.h CarModel.cpp
    MainWindow.h MainWindow.cpp

MainWindow acts as a controller of simulation progress.
Select "Normal" to start a regular simulation, then try any of the safety features.


To control program flow manually (in normal use case) you can disable the timer by commenting out line 127 of MainWindow.cpp

Diagrams are included in the diagrams folder, which include my updated
Assignment 1 diagrams.

Normal use case scenario works by creating a list of passengers, seeing if they can be added to any of the elevators upcoming_passengers using the determine_elevator() function.  Once a passenger is added to an elevators upcoming_passengers, that elevator adds the current floor of that passenger to its set of destinations.  When it reaches that floor, the passenger is added to the passengers list, and that passengers final destination is added to the elevators destination.

Each cycle of the normal use case in MainWindow tries to move the elevators.  It is up to the elevator whether it moves or not, in order to follow its safety features.

The Overload, Help and Door Obstacle buttons act as a toggle on off.  Pressing the button once will cause the Safety Feature to engage, pushing the button again will toggle it back off.
