#pragma once
#include "Interface/IEasy.hpp"
class EasyBase : public IEasy{
    protected:
    explicit EasyBase(int easy);
    public:
    int get_easy() const override;
    private:
    int easy_;
};