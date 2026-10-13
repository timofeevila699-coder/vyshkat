#include <boost/di.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include "Console/ConsoleApp.hpp"
#include "Park/Park.hpp"
#include "Report/Report.hpp"
#include "ServiceCenter/ServiceCenter.hpp"
namespace di = boost::di;
constexpr double kMaxEnergy = 2.0;
constexpr int kMinEasy = 2;
int main() {
    try {
        auto service_center =
            std::make_shared<ServiceCenter>(kMaxEnergy, kMinEasy);
        auto injector = di::make_injector(
            di::bind<IServiceCenter>.to(service_center),
            di::bind<Park>.in(di::singleton),
            di::bind<Report>.in(di::singleton));
        auto app = injector.create<ConsoleApp>();
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << '\n';
        return 1;
    }
    return 0;
}