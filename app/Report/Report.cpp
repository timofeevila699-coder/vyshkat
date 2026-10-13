#include "Report.hpp"
#include "Interface/IEnergy.hpp"
#include "Interface/IEasy.hpp"
Report::Report(const Park& park): park_(park){}
std::size_t Report::count_devices() const{return park_.items().size();}
double Report::sum_energy() const{
    double result = 0;
    for (const auto& item : park_.items()){
        if (const auto* energy = dynamic_cast<const IEnergy*>(item)){
            result += energy->get_energy();
        }
    }
    return result;
}
std::vector<const IInventory*> Report::list_beginners() const{
    std::vector<const IInventory*> result;
    for (const auto& item : park_.items()){
        if (const auto* easy = dynamic_cast<const IEasy*>(item)){
            if (easy->get_easy() >= kBeginnerMinEasy){
                result.push_back(item);
            }
        }
    }
    return result;
}
std::vector<const IInventory*> Report::sheet() const{
    return park_.items();
}