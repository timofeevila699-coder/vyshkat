#pragma once
#include "Park/Park.hpp"
#include "Interface/IInventory.hpp"
#include <vector>
class Report {
    public:
    static constexpr int kBeginnerMinEasy = 6;
    explicit Report(const Park& park);
    std::size_t count_devices() const;
    double sum_energy() const;
    std::vector<const IInventory*> list_beginners() const;
    std::vector<const IInventory*> sheet() const;
    private:
    const Park& park_;
};