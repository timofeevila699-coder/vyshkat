#pragma once
#include<string>
#include<optional>
class IInventory{
    public:
    virtual ~IInventory() = default;
    virtual std::optional<long long> get_number() const = 0;
    virtual std::string get_name() const = 0;
    virtual bool set_number(long long x) = 0;
};