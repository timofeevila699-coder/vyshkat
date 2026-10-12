#include <boost/di.hpp>
#include <iostream>
#include "Devices/Bicycle.hpp"
#include "Devices/ChargingCabinet.hpp"
#include "Devices/DockingStation.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/Helmet.hpp"
int main() {
    std::cout << "ВышКат\n";
    Bicycle bicycle("Велосипед" , 6);
    ChargingCabinet chargingcabinet("Электрический кабинет" , 123.5);
    DockingStation dockingstation("Докинг стейшн" , 12.6);
    ElectricBicycle electricbicycle("Электрический велосипед" , 45.9 , 4);
    ElectricScooter electricscooter("Электрический скутер", 65.8, 8);
    Helmet helmet("Шлем");
    return 0;
}