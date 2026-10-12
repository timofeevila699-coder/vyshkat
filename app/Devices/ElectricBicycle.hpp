#pragma once
#include <string>
#include <utility>
#include "Base/EasyBase/EasyBase.hpp"
#include "Base/EnergyBase/EnergyBase.hpp"
#include "Base/InventoryBase/InventoryBase.hpp"
class ElectricBicycle : public InventoryBase , public EnergyBase , public EasyBase{
    public:
    ElectricBicycle(std::string name , double energy , int easy): InventoryBase(std::move(name)) , EnergyBase(energy) , EasyBase(easy){}
};