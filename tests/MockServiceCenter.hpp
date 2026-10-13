#pragma once
#include <gmock/gmock.h>
#include "Interface/IServiceCenter.hpp"
#include "Interface/IInventory.hpp"

class MockServiceCenter : public IServiceCenter {
public:
    MOCK_METHOD(bool, approve_device, (const IInventory& device), (const, override));
};
