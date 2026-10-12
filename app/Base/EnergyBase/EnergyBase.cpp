#include "EnergyBase.hpp"
#include <stdexcept>
EnergyBase::EnergyBase(double energy):energy_(energy){
    if (energy_ < 0){
        throw std::invalid_argument("Отрицательная энергия!");
    }
}
double EnergyBase::get_energy() const{ return energy_;}