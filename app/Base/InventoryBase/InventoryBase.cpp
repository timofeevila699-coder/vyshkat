#include "InventoryBase.hpp"
#include <utility>
#include <stdexcept>
InventoryBase::InventoryBase(std::string name):name_(std::move(name)){
    if(name_.empty()){
        throw std::invalid_argument("Пустое название!");
    }
}
std::optional<long long> InventoryBase::get_number() const{ return number_;}
std::string InventoryBase::get_name() const{return name_;}
bool InventoryBase::set_number(long long number){
    if(number_.has_value()){
        return false;
    }
    else{
        number_ = number;
        return true;
    }
}
