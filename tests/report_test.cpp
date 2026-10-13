#include <gtest/gtest.h>
#include "Report/Report.hpp"
#include "Park/Park.hpp"
#include "ServiceCenter/ServiceCenter.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/Bicycle.hpp"
#include "Devices/Helmet.hpp"
#include "Interface/IEasy.hpp"

class ReportTest : public ::testing::Test {
protected:
    std::shared_ptr<IServiceCenter> service_center_{
        std::make_shared<ServiceCenter>(200.0, 2)
    };
    Park park_{service_center_};
    Report report_{park_};

    void SetUp() override {
        auto scooter1 = std::make_unique<ElectricScooter>("Самокат 1", 50.0, 8);
        auto scooter2 = std::make_unique<ElectricScooter>("Самокат 2", 75.0, 5);
        auto ebike = std::make_unique<ElectricBicycle>("E-велик", 100.0, 6);
        auto bicycle = std::make_unique<Bicycle>("Велик", 7);
        auto helmet = std::make_unique<Helmet>("Шлем");

        EXPECT_TRUE(park_.add(std::move(scooter1)));
        EXPECT_TRUE(park_.add(std::move(scooter2)));
        EXPECT_TRUE(park_.add(std::move(ebike)));
        EXPECT_TRUE(park_.add(std::move(bicycle)));
        EXPECT_TRUE(park_.add(std::move(helmet)));
    }
};

TEST_F(ReportTest, CountDevicesReturnsCorrectNumber) {
    EXPECT_EQ(report_.count_devices(), 5);
}

TEST_F(ReportTest, SumEnergyCalculatesCorrectly) {
    EXPECT_DOUBLE_EQ(report_.sum_energy(), 225.0);
}

TEST_F(ReportTest, ListBeginnersFiltersCorrectly) {
    // В фикстуре: Самокат1(8), E-велик(6), Велик(7) = 3 новичка
    auto beginners = report_.list_beginners();
    EXPECT_EQ(beginners.size(), 3);
}

TEST_F(ReportTest, ListBeginnersContainsHighEasyDevices) {
    auto beginners = report_.list_beginners();
    for (const auto& device : beginners) {
        if (const auto* easy = dynamic_cast<const IEasy*>(device)) {
            EXPECT_GE(easy->get_easy(), 6);
        }
    }
}

TEST_F(ReportTest, BeginnerBoundaryEasy6Included) {
    // Граница 6 должна быть включена
    auto beginners = report_.list_beginners();
    bool found_easy_6 = false;
    for (const auto& device : beginners) {
        if (const auto* easy = dynamic_cast<const IEasy*>(device)) {
            if (easy->get_easy() == 6) {
                found_easy_6 = true;
            }
        }
    }
    EXPECT_TRUE(found_easy_6);
}

TEST_F(ReportTest, BeginnerBoundaryEasy5Excluded) {
    // Граница 5 должна быть исключена, но это проверяется через scooter2 с easy=5
    // Проверяем, что в списке новичков его нет
    auto beginners = report_.list_beginners();
    for (const auto& device : beginners) {
        EXPECT_NE(device->get_name(), "Самокат 2");
    }
}

TEST_F(ReportTest, SheetReturnsAllItems) {
    auto sheet = report_.sheet();
    EXPECT_EQ(sheet.size(), 5);
}

TEST(ReportEmptyParkTest, CountDevices) {
    auto sc = std::make_shared<ServiceCenter>(100.0, 1);
    Park empty_park(sc);
    Report empty_report(empty_park);

    EXPECT_EQ(empty_report.count_devices(), 0);
}

TEST(ReportEmptyParkTest, SumEnergy) {
    auto sc = std::make_shared<ServiceCenter>(100.0, 1);
    Park empty_park(sc);
    Report empty_report(empty_park);

    EXPECT_DOUBLE_EQ(empty_report.sum_energy(), 0.0);
}

TEST(ReportEmptyParkTest, ListBeginnersEmpty) {
    auto sc = std::make_shared<ServiceCenter>(100.0, 1);
    Park empty_park(sc);
    Report empty_report(empty_park);

    EXPECT_EQ(empty_report.list_beginners().size(), 0);
}

TEST(ReportNoBeginnerTest, ListBeginnersEmptyWhenNoDevicesQualify) {
    auto sc = std::make_shared<ServiceCenter>(200.0, 2);
    Park park(sc);

    auto bike1 = std::make_unique<Bicycle>("Велик1", 3);
    auto bike2 = std::make_unique<Bicycle>("Велик2", 4);

    EXPECT_TRUE(park.add(std::move(bike1)));
    EXPECT_TRUE(park.add(std::move(bike2)));

    Report report(park);
    EXPECT_EQ(report.list_beginners().size(), 0);
}
