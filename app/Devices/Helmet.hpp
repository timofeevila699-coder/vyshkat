#pragma once
#include <string>
#include <utility>
#include "Base/InventoryBase/InventoryBase.hpp"
class Helmet : public InventoryBase {
    public:
    explicit Helmet(std::string name):InventoryBase(std::move(name)){}
};