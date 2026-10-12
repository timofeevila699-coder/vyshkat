#pragma once
class IEasy{
    public:
    virtual ~IEasy() = default;
    virtual int get_easy() const = 0;
};