#pragma once
#include "IInventory.hpp"
class IServiceCenter{
    public:
    virtual ~IServiceCenter() = default;
    [[nodiscard]] virtual bool approve_device(const IInventory& device) const = 0;
};