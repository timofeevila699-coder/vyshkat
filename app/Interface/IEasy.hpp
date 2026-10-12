#pragma once
class IEasy{
    public:
    static constexpr int kMinEasy = 1;
    static constexpr int kMaxEasy = 10;
    virtual ~IEasy() = default;
    virtual int get_easy() const = 0;
};