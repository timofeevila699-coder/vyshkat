#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "Park/Park.hpp"
#include "ServiceCenter/ServiceCenter.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/Bicycle.hpp"
#include "Devices/Helmet.hpp"
#include "MockServiceCenter.hpp"

class ParkTest : public ::testing::Test {
protected:
    std::shared_ptr<IServiceCenter> service_center_{
        std::make_shared<ServiceCenter>(100.0, 3)
    };
    Park park_{service_center_};
};

TEST_F(ParkTest, ParkConstructorWithNullServiceCenter) {
    std::shared_ptr<IServiceCenter> null_center = nullptr;
    EXPECT_THROW(Park p(null_center), std::invalid_argument);
}

TEST_F(ParkTest, AddDeviceSuccessfully) {
    auto scooter = std::make_unique<ElectricScooter>("Самокат", 50.0, 5);
    EXPECT_TRUE(park_.add(std::move(scooter)));
}

TEST_F(ParkTest, AddDeviceWithRejection) {
    auto scooter = std::make_unique<ElectricScooter>("Самокат", 150.0, 5);
    EXPECT_FALSE(park_.add(std::move(scooter)));
}

TEST_F(ParkTest, AddMultipleDevices) {
    auto scooter1 = std::make_unique<ElectricScooter>("Самокат 1", 50.0, 5);
    auto scooter2 = std::make_unique<ElectricScooter>("Самокат 2", 60.0, 7);
    auto helmet = std::make_unique<Helmet>("Шлем");

    EXPECT_TRUE(park_.add(std::move(scooter1)));
    EXPECT_TRUE(park_.add(std::move(scooter2)));
    EXPECT_TRUE(park_.add(std::move(helmet)));

    EXPECT_EQ(park_.items().size(), 3);
}

TEST_F(ParkTest, ItemsReturnsCorrectInventoryNumbers) {
    auto scooter1 = std::make_unique<ElectricScooter>("Самокат 1", 50.0, 5);
    auto scooter2 = std::make_unique<ElectricScooter>("Самокат 2", 60.0, 5);

    EXPECT_TRUE(park_.add(std::move(scooter1)));
    EXPECT_TRUE(park_.add(std::move(scooter2)));

    auto items = park_.items();
    ASSERT_EQ(items.size(), 2);
    EXPECT_EQ(items[0]->get_number(), 1);
    EXPECT_EQ(items[1]->get_number(), 2);
}

TEST_F(ParkTest, AddNullDevice) {
    EXPECT_FALSE(park_.add(nullptr));
}

TEST_F(ParkTest, ItemsEmptyParkReturnsEmptyVector) {
    EXPECT_EQ(park_.items().size(), 0);
}

TEST_F(ParkTest, RejectedDeviceDoesNotConsumeInventoryNumber) {
    // Добавляем устройство, которое отклонится
    auto rejected = std::make_unique<ElectricScooter>("Отклонён", 150.0, 5);
    EXPECT_FALSE(park_.add(std::move(rejected)));

    // Добавляем принятое устройство
    auto accepted = std::make_unique<Bicycle>("Принят", 5);
    EXPECT_TRUE(park_.add(std::move(accepted)));

    // Отклонённое устройство не израсходовало номер: принятое получает 1
    auto items = park_.items();
    ASSERT_EQ(items.size(), 1);
    EXPECT_EQ(items[0]->get_number(), 1);
}

TEST_F(ParkTest, AddWithMockServiceCenterAccepts) {
    auto mock_center = std::make_shared<MockServiceCenter>();
    Park park_with_mock(mock_center);

    EXPECT_CALL(*mock_center, approve_device(::testing::_))
        .WillOnce(::testing::Return(true));

    auto scooter = std::make_unique<ElectricScooter>("Самокат", 50.0, 5);
    EXPECT_TRUE(park_with_mock.add(std::move(scooter)));
    EXPECT_EQ(park_with_mock.items().size(), 1);
}

TEST_F(ParkTest, AddWithMockServiceCenterRejects) {
    auto mock_center = std::make_shared<MockServiceCenter>();
    Park park_with_mock(mock_center);

    EXPECT_CALL(*mock_center, approve_device(::testing::_))
        .WillOnce(::testing::Return(false));

    auto scooter = std::make_unique<ElectricScooter>("Самокат", 50.0, 5);
    EXPECT_FALSE(park_with_mock.add(std::move(scooter)));
    EXPECT_EQ(park_with_mock.items().size(), 0);
}

TEST_F(ParkTest, MockDemonstrateDependencyInjection) {
    // Мок подменяет реальный сервис-центр через DI.
    // Мок принимает устройство, которое должен отклонить настоящий центр.
    auto mock_center = std::make_shared<MockServiceCenter>();
    Park park_with_mock(mock_center);

    EXPECT_CALL(*mock_center, approve_device(::testing::_))
        .WillOnce(::testing::Return(true));

    // Самокат с избыточным энергопотреблением (настоящий центр отклонил бы)
    auto overpower_scooter = std::make_unique<ElectricScooter>("Мега-самокат", 9999.0, 1);
    EXPECT_TRUE(park_with_mock.add(std::move(overpower_scooter)));

    // Это доказывает, что парк полагается на внедрённое решение
    auto items = park_with_mock.items();
    ASSERT_EQ(items.size(), 1);
    EXPECT_EQ(items[0]->get_name(), "Мега-самокат");
}
