#include <gtest/gtest.h>
#include "Park/Park.hpp"
#include "Report/Report.hpp"
#include "ServiceCenter/ServiceCenter.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/Bicycle.hpp"
#include "Devices/Helmet.hpp"
#include "Devices/ChargingCabinet.hpp"

class IntegrationTest : public ::testing::Test {
protected:
    std::shared_ptr<IServiceCenter> service_center_{
        std::make_shared<ServiceCenter>(150.0, 4)
    };
    Park park_{service_center_};
};

TEST_F(IntegrationTest, FullWorkflowAddDevicesAndGenerateReports) {
    auto scooter = std::make_unique<ElectricScooter>("Самокат", 60.0, 8);
    auto ebike = std::make_unique<ElectricBicycle>("E-велик", 90.0, 5);
    auto helmet = std::make_unique<Helmet>("Шлем");

    EXPECT_TRUE(park_.add(std::move(scooter)));
    EXPECT_TRUE(park_.add(std::move(ebike)));
    EXPECT_TRUE(park_.add(std::move(helmet)));

    Report report(park_);

    EXPECT_EQ(report.count_devices(), 3);
    EXPECT_DOUBLE_EQ(report.sum_energy(), 150.0);
    EXPECT_EQ(report.list_beginners().size(), 1);
}

TEST_F(IntegrationTest, RejectedDeviceNotAddedToPark) {
    auto rejected_scooter = std::make_unique<ElectricScooter>("Самокат", 200.0, 8);
    auto accepted_bike = std::make_unique<Bicycle>("Велик", 5);

    EXPECT_FALSE(park_.add(std::move(rejected_scooter)));
    EXPECT_TRUE(park_.add(std::move(accepted_bike)));

    EXPECT_EQ(park_.items().size(), 1);
}

TEST_F(IntegrationTest, InventoryNumbersAreSequential) {
    auto dev1 = std::make_unique<ElectricScooter>("Самокат 1", 50.0, 5);
    auto dev2 = std::make_unique<Bicycle>("Велик", 6);
    auto dev3 = std::make_unique<Helmet>("Шлем");

    EXPECT_TRUE(park_.add(std::move(dev1)));
    EXPECT_TRUE(park_.add(std::move(dev2)));
    EXPECT_TRUE(park_.add(std::move(dev3)));

    auto items = park_.items();
    ASSERT_EQ(items.size(), 3);
    for (size_t i = 0; i < items.size(); ++i) {
        EXPECT_EQ(items[i]->get_number().value(), static_cast<long long>(i + 1));
    }
}

TEST_F(IntegrationTest, ReportAccuracyWithMixedDevices) {
    auto devices = std::vector<std::unique_ptr<IInventory>>();
    devices.push_back(std::make_unique<ElectricScooter>("Самокат", 50.0, 8));
    devices.push_back(std::make_unique<ElectricBicycle>("E-велик", 100.0, 6));
    devices.push_back(std::make_unique<Bicycle>("Велик", 5));
    devices.push_back(std::make_unique<Helmet>("Шлем"));
    devices.push_back(std::make_unique<ChargingCabinet>("Зарядка", 120.0));

    for (auto& device : devices) {
        EXPECT_TRUE(park_.add(std::move(device)));
    }

    Report report(park_);

    EXPECT_EQ(report.count_devices(), 5);
    EXPECT_DOUBLE_EQ(report.sum_energy(), 270.0);
    EXPECT_EQ(report.list_beginners().size(), 2);
}

TEST_F(IntegrationTest, DifferentServiceCenterPolicies) {
    auto strict_center = std::make_shared<ServiceCenter>(50.0, 8);
    Park strict_park(strict_center);

    // Отклоняется по энергии: 60 > max_energy = 50 (простота 9 >= 8 в норме)
    auto device1 = std::make_unique<ElectricScooter>("Самокат", 60.0, 9);
    EXPECT_FALSE(strict_park.add(std::move(device1)));

    // Отклоняется по простоте: 7 < min_easy = 8 (энергия 30 <= 50 в норме)
    auto device2 = std::make_unique<ElectricScooter>("Самокат", 30.0, 7);
    EXPECT_FALSE(strict_park.add(std::move(device2)));
}
