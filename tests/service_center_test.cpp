#include <gtest/gtest.h>
#include "ServiceCenter/ServiceCenter.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/Bicycle.hpp"
#include "Devices/Helmet.hpp"

class ServiceCenterTest : public ::testing::Test {
protected:
    ServiceCenter service_center_{100.0, 3};
};

TEST_F(ServiceCenterTest, ConstructorValidParameters) {
    EXPECT_NO_THROW(ServiceCenter(50.0, 5));
}

TEST_F(ServiceCenterTest, ConstructorInvalidMaxEnergy) {
    EXPECT_THROW(ServiceCenter(0, 5), std::invalid_argument);
    EXPECT_THROW(ServiceCenter(-10, 5), std::invalid_argument);
}

TEST_F(ServiceCenterTest, ConstructorInvalidMinEasy) {
    EXPECT_THROW(ServiceCenter(100, 0), std::invalid_argument);
    EXPECT_THROW(ServiceCenter(100, 11), std::invalid_argument);
}

TEST_F(ServiceCenterTest, ApproveDeviceWithinEnergyLimit) {
    ElectricScooter scooter("Самокат", 50.0, 5);
    EXPECT_TRUE(service_center_.approve_device(scooter));
}

TEST_F(ServiceCenterTest, ApproveDeviceExceedsEnergyLimit) {
    ElectricScooter scooter("Самокат", 150.0, 5);
    EXPECT_FALSE(service_center_.approve_device(scooter));
}

TEST_F(ServiceCenterTest, ApproveDeviceWithinEasyLimit) {
    Bicycle bicycle("Велик", 5);
    EXPECT_TRUE(service_center_.approve_device(bicycle));
}

TEST_F(ServiceCenterTest, ApproveDeviceBelowEasyLimit) {
    Bicycle bicycle("Велик", 2);
    EXPECT_FALSE(service_center_.approve_device(bicycle));
}

TEST_F(ServiceCenterTest, ApproveDeviceNoEnergyInterface) {
    Helmet helmet("Шлем");
    EXPECT_TRUE(service_center_.approve_device(helmet));
}

TEST_F(ServiceCenterTest, ApproveDeviceMultipleInterfaces) {
    ElectricBicycle ebike("E-велик", 75.0, 7);
    EXPECT_TRUE(service_center_.approve_device(ebike));
}

TEST_F(ServiceCenterTest, ApproveDeviceOnEnergyBoundary) {
    ElectricScooter scooter("Самокат", 100.0, 5);
    EXPECT_TRUE(service_center_.approve_device(scooter));
}

TEST_F(ServiceCenterTest, ApproveDeviceOnEasyBoundary) {
    Bicycle bicycle("Велик", 3);
    EXPECT_TRUE(service_center_.approve_device(bicycle));
}
