#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/di.hpp>
#include <memory>

#include "Devices/ElectricScooter.hpp"
#include "Interface/IServiceCenter.hpp"
#include "Park/Park.hpp"

namespace di = boost::di;

namespace {

class FakeServiceCenter : public IServiceCenter {
public:
    MOCK_METHOD(bool, approve_device, (const IInventory& device), (const, override));
};

}  // namespace

// Тот же способ сборки, что в main.cpp, но вместо настоящего техосмотра
// контейнеру отдаётся мок. Парк получает его, ничего не зная о подмене.
TEST(DiContainerTest, ContainerInjectsFakeServiceCenterIntoPark) {
    auto fake_center = std::make_shared<FakeServiceCenter>();
    EXPECT_CALL(*fake_center, approve_device(::testing::_))
        .WillOnce(::testing::Return(false));

    auto injector = di::make_injector(
        di::bind<IServiceCenter>.to(fake_center),
        di::bind<Park>.in(di::singleton));

    Park& park = injector.create<Park&>();

    // Настоящий центр принял бы такой самокат, но решает подменённый
    EXPECT_FALSE(park.add(std::make_unique<ElectricScooter>("Самокат", 0.4, 7)));
    EXPECT_TRUE(park.items().empty());
}

// Консоль и отчёт должны работать с одним и тем же парком
TEST(DiContainerTest, ParkIsSingletonInContainer) {
    auto fake_center = std::make_shared<FakeServiceCenter>();
    auto injector = di::make_injector(
        di::bind<IServiceCenter>.to(fake_center),
        di::bind<Park>.in(di::singleton));

    Park& first = injector.create<Park&>();
    Park& second = injector.create<Park&>();
    EXPECT_EQ(&first, &second);
}
