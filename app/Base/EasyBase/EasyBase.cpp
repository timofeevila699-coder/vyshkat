#include "EasyBase.hpp"
#include <stdexcept>
EasyBase::EasyBase(int easy): easy_(easy){
    if(easy_ < IEasy::kMinEasy || easy_ > IEasy::kMaxEasy){
        throw std::invalid_argument("Простота должна быть от 1 до 10");
    }
}
int EasyBase::get_easy() const{return easy_;}