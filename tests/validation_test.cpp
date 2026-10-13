#include <gtest/gtest.h>
#include "Devices/ElectricScooter.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/Bicycle.hpp"
#include "Devices/Helmet.hpp"
#include "Devices/ChargingCabinet.hpp"
#include "Devices/DockingStation.hpp"

// ============================================================================
// Тесты валидации базовых классов
// ============================================================================

TEST(ValidationTest, EnergyBaseNegativeEnergy) {
    EXPECT_THROW(ElectricScooter("Самокат", -50.0, 5), std::invalid_argument);
}

TEST(ValidationTest, EasyBaseBelowMinimum) {
    EXPECT_THROW(ElectricBicycle("E-велик", 50.0, 0), std::invalid_argument);
}

TEST(ValidationTest, EasyBaseAboveMaximum) {
    EXPECT_THROW(Bicycle("Велик", 11), std::invalid_argument);
}

TEST(ValidationTest, InventoryBaseEmptyName) {
    EXPECT_THROW(Helmet(""), std::invalid_argument);
}

// ============================================================================
// Тесты Devices (устройства)
// ============================================================================

class DevicesTest : public ::testing::Test {};

TEST_F(DevicesTest, ElectricScooterCreation) {
    ElectricScooter scooter("Самокат", 50.0, 7);
    EXPECT_EQ(scooter.get_name(), "Самокат");
    EXPECT_DOUBLE_EQ(scooter.get_energy(), 50.0);
    EXPECT_EQ(scooter.get_easy(), 7);
}

TEST_F(DevicesTest, ElectricBicycleCreation) {
    ElectricBicycle ebike("E-велик", 100.0, 5);
    EXPECT_EQ(ebike.get_name(), "E-велик");
    EXPECT_DOUBLE_EQ(ebike.get_energy(), 100.0);
    EXPECT_EQ(ebike.get_easy(), 5);
}

TEST_F(DevicesTest, BicycleCreation) {
    Bicycle bike("Велик", 6);
    EXPECT_EQ(bike.get_name(), "Велик");
    EXPECT_EQ(bike.get_easy(), 6);
}

TEST_F(DevicesTest, HelmetCreation) {
    Helmet helmet("Шлем");
    EXPECT_EQ(helmet.get_name(), "Шлем");
}

TEST_F(DevicesTest, ChargingCabinetCreation) {
    ChargingCabinet cabinet("Зарядка", 120.0);
    EXPECT_EQ(cabinet.get_name(), "Зарядка");
    EXPECT_DOUBLE_EQ(cabinet.get_energy(), 120.0);
}

TEST_F(DevicesTest, DockingStationCreation) {
    DockingStation station("Парковка", 80.0);
    EXPECT_EQ(station.get_name(), "Парковка");
    EXPECT_DOUBLE_EQ(station.get_energy(), 80.0);
}

TEST_F(DevicesTest, InventoryNumberAssignment) {
    ElectricScooter scooter("Самокат", 50.0, 7);
    EXPECT_FALSE(scooter.get_number().has_value());

    EXPECT_TRUE(scooter.set_number(42));
    EXPECT_TRUE(scooter.get_number().has_value());
    EXPECT_EQ(scooter.get_number().value(), 42);
}

TEST_F(DevicesTest, InventoryNumberCannotBeSetTwice) {
    ElectricScooter scooter("Самокат", 50.0, 7);
    EXPECT_TRUE(scooter.set_number(1));
    EXPECT_FALSE(scooter.set_number(2));
    EXPECT_EQ(scooter.get_number().value(), 1);
}
