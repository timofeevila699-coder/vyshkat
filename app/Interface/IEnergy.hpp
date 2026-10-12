#pragma once
class IEnergy {
    public:
    virtual ~IEnergy() = default;
    virtual double get_energy() const = 0;
};