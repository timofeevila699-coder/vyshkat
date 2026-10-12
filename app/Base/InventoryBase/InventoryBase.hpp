#pragma once
#include "Interface/IInventory.hpp"
#include <optional>
#include <string>

class InventoryBase : public IInventory {
    protected:
    explicit InventoryBase(std::string name);
    public:
    std::optional<long long> get_number() const override;
    std::string get_name() const override;
    bool set_number(long long x) override;
    private:
    std::string name_;
    std::optional<long long> number_;
};