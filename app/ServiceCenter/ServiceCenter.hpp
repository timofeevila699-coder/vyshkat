#pragma once
#include "Interface/IServiceCenter.hpp"
class ServiceCenter : public IServiceCenter {
    public:
    ServiceCenter(double energy , int min_easy);
    [[nodiscard]] bool approve_device(const IInventory& device) const override;
    private:
    double max_energy_;
    int min_easy_;
};