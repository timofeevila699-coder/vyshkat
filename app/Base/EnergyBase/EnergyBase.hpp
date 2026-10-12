#pragma once
#include"Interface/IEnergy.hpp"
class EnergyBase : public IEnergy{
    public:
    double get_energy() const override;
    protected:
    explicit EnergyBase(double energy);
    private:
    double energy_;
};