#pragma once
#include "Interface/IServiceCenter.hpp"
#include "Interface/IInventory.hpp"
#include <vector>
#include <memory>
class Park{
    public:
    explicit Park(std::shared_ptr<IServiceCenter> service_center);
    [[nodiscard]] bool add(std::unique_ptr<IInventory> device);
    std::vector<const IInventory*> items() const;
    private:
    std::shared_ptr<IServiceCenter> service_center_;
    std::vector<std::unique_ptr<IInventory>> items_;
    long long next_number_ = 1;
};