#pragma once
#include <string>
#include <utility>
#include "Base/EnergyBase/EnergyBase.hpp"
#include "Base/InventoryBase/InventoryBase.hpp"
class ChargingCabinet : public InventoryBase , public EnergyBase{
    public:
    ChargingCabinet(std::string name , double energy): InventoryBase(std::move(name)) , EnergyBase(energy){}
};