#include "ServiceCenter.hpp"
#include "Interface/IEnergy.hpp"
#include "Interface/IEasy.hpp"
#include <stdexcept>
ServiceCenter::ServiceCenter(double max_energy , int min_easy): max_energy_(max_energy) , min_easy_(min_easy){
    if(max_energy <= 0){throw std::invalid_argument("Порог энергии должен быть больше нуля");}
    if(min_easy < IEasy::kMinEasy || min_easy > IEasy::kMaxEasy){throw std::invalid_argument("Простота должна быть в диапазоне от 1 до 10");}
}
bool ServiceCenter::approve_device(const IInventory& device) const {
    if(const auto* energy_interface = dynamic_cast<const IEnergy*>(&device)){
        double energy = energy_interface->get_energy();
        if (energy > max_energy_){return false;}
    }
    if(const auto* easy_interface = dynamic_cast<const IEasy*>(&device)){
        int easy = easy_interface->get_easy();
        if (easy < min_easy_){return false;}
    }
    return true;
}