#include "Park.hpp"
#include <stdexcept>
#include <utility>
Park::Park(std::shared_ptr<IServiceCenter> service_center): service_center_(std::move(service_center)){
    if(service_center_ == nullptr){throw std::invalid_argument("Пустой указатель на сервис центр");}
}
bool Park::add(std::unique_ptr<IInventory> device){
    if(device == nullptr){return false;}
    if(service_center_->approve_device(*device)){
        if(device->set_number(next_number_)){
            ++next_number_;
            items_.push_back(std::move(device));
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
std::vector<const IInventory*> Park::items() const{
    std::vector<const IInventory*> result;
    result.reserve(items_.size());
    for(const auto& item : items_){
        result.push_back(item.get());
    }
    return result;
}