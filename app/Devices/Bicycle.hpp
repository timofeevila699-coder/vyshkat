#pragma once
#include <string>
#include <utility>
#include "Base/EasyBase/EasyBase.hpp"
#include "Base/InventoryBase/InventoryBase.hpp"
class Bicycle : public InventoryBase , public EasyBase {
    public:
    Bicycle(std::string name , int easy): InventoryBase(std::move(name)) , EasyBase(easy){}
};