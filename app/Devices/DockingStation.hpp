#pragma once
#include <string>
#include <utility>
#include "Base/EnergyBase/EnergyBase.hpp"
#include "Base/InventoryBase/InventoryBase.hpp"
class DockingStation : public InventoryBase , public EnergyBase{
    public:
    DockingStation(std::string name , double energy): InventoryBase(std::move(name)) , EnergyBase(energy){}
};