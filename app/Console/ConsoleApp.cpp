#include "Console/ConsoleApp.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include "Devices/Bicycle.hpp"
#include "Devices/ChargingCabinet.hpp"
#include "Devices/DockingStation.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/Helmet.hpp"
namespace {
std::optional<std::string> read_line(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt;
    }
    return line;
}
template <typename T>
std::optional<T> read_number(const std::string& prompt) {
    while (true) {
        auto line = read_line(prompt);
        if (!line) {
            return std::nullopt;
        }
        if constexpr (std::is_floating_point_v<T>) {
            std::replace(line->begin(), line->end(), ',', '.');
        }
        std::istringstream in(*line);
        T value{};
        if (in >> value && (in >> std::ws).eof()) {
            if constexpr (std::is_floating_point_v<T>) {
                if (!std::isfinite(value)) {
                    std::cout << "Нужно ввести конечное число.\n";
                    continue;
                }
            }
            return value;
        }
        std::cout << "Нужно ввести число.\n";
    }
}
struct DeviceKind {
    const char* title;
    bool needs_energy;
    bool needs_easy;
    std::unique_ptr<IInventory> (*make)(const std::string& name, double energy, int easy);
};
const std::array<DeviceKind, 6> kDeviceKinds = {{
    {"Электросамокат", true, true,
     [](const std::string& name, double energy,
        int easy) -> std::unique_ptr<IInventory> {
         return std::make_unique<ElectricScooter>(name, energy, easy);
     }},
    {"Электровелосипед", true, true,
     [](const std::string& name, double energy,
        int easy) -> std::unique_ptr<IInventory> {
         return std::make_unique<ElectricBicycle>(name, energy, easy);
     }},
    {"Велосипед", false, true,
     [](const std::string& name, double,
        int easy) -> std::unique_ptr<IInventory> {
         return std::make_unique<Bicycle>(name, easy);
     }},
    {"Шлем", false, false,
     [](const std::string& name, double, int) -> std::unique_ptr<IInventory> {
         return std::make_unique<Helmet>(name);
     }},
    {"Док-станция", true, false,
     [](const std::string& name, double energy,
        int) -> std::unique_ptr<IInventory> {
         return std::make_unique<DockingStation>(name, energy);
     }},
    {"Зарядный шкаф", true, false,
     [](const std::string& name, double energy,
        int) -> std::unique_ptr<IInventory> {
         return std::make_unique<ChargingCabinet>(name, energy);
     }},
}};
std::string format_number(const IInventory& item) {
    if (auto number = item.get_number()) {
        return "№" + std::to_string(*number);
    }
    return "без номера";
}
void print_items(const std::vector<const IInventory*>& items,
                 const std::string& empty_message) {
    if (items.empty()) {
        std::cout << "  " << empty_message << '\n';
        return;
    }
    for (const auto* item : items) {
        std::cout << "  " << std::setw(6) << std::left << format_number(*item)
                  << ' ' << item->get_name() << '\n';
    }
}
}
ConsoleApp::ConsoleApp(Park& park, const Report& report): park_(park), report_(report) {}
void ConsoleApp::run() {
    while (true) {
        print_menu();
        auto choice = read_number<int>("> ");
        if (!choice || *choice == 0) {
            std::cout << "До свидания!\n";
            return;
        }
        switch (*choice) {
            case 1: add_device(); break;
            case 2: show_report(); break;
            case 3: show_beginners(); break;
            case 4: show_sheet(); break;
            case 5: run_demo(); break;
            default: std::cout << "Нет такого пункта.\n";
        }
    }
}
void ConsoleApp::print_menu() const {
    std::cout << "\n=== ВышКат: учёт парка ===\n"
              << "1. Добавить устройство\n"
              << "2. Отчёт: число единиц и энергия\n"
              << "3. Список для новичков\n"
              << "4. Инвентарная ведомость\n"
              << "5. Демо-сценарий\n"
              << "0. Выход\n";
}
void ConsoleApp::add_device() {
    std::cout << "\nТип устройства:\n";
    for (std::size_t i = 0; i < kDeviceKinds.size(); ++i) {
        std::cout << i + 1 << ". " << kDeviceKinds[i].title << '\n';
    }
    auto type = read_number<int>("Тип: ");
    if (!type) {
        return;
    }
    if (*type < 1 || *type > static_cast<int>(kDeviceKinds.size())) {
        std::cout << "Нет такого типа.\n";
        return;
    }
    const DeviceKind& kind = kDeviceKinds[static_cast<std::size_t>(*type - 1)];
    auto name = read_line("Название: ");
    if (!name) {
        return;
    }
    double energy = 0;
    if (kind.needs_energy) {
        auto value = read_number<double>("Суточный расход, кВт·ч: ");
        if (!value) {
            return;
        }
        energy = *value;
    }
    int easy = IEasy::kMinEasy;
    if (kind.needs_easy) {
        auto value = read_number<int>("Простота (1–10): ");
        if (!value) {
            return;
        }
        easy = *value;
    }
    std::unique_ptr<IInventory> device;
    try {
        device = kind.make(*name, energy, easy);
    } catch (const std::invalid_argument& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
        return;
    }
    accept(std::move(device));
}
void ConsoleApp::accept(std::unique_ptr<IInventory> device) {
    const std::string name = device->get_name();
    if (park_.add(std::move(device))) {
        std::cout << "Принято: «" << name << "», "
                  << format_number(*park_.items().back()) << '\n';
    } else {
        std::cout << "Отклонено техосмотром: «" << name << "»\n";
    }
}
void ConsoleApp::show_report() const {
    std::cout << "\nОтчёт по парку:\n"
              << "  Единиц на балансе: " << report_.count_devices() << '\n'
              << "  Суточное потребление: " << std::fixed
              << std::setprecision(2) << report_.sum_energy() << " кВт·ч\n";
}
void ConsoleApp::show_beginners() const {
    std::cout << "\nМожно выдавать новичкам (простота ≥ "
              << Report::kBeginnerMinEasy << "):\n";
    print_items(report_.list_beginners(), "Подходящих устройств нет.");
}
void ConsoleApp::show_sheet() const {
    std::cout << "\nИнвентарная ведомость:\n";
    print_items(report_.sheet(), "Парк пуст.");
}
void ConsoleApp::run_demo() {
    std::cout << "\n=== Демо-сценарий ===\n"
              << "\n-- Приём техники и вещей --\n";
    accept(std::make_unique<ElectricScooter>("Ninebot Max", 0.4, 7));
    accept(std::make_unique<ElectricBicycle>("Eltreco XT", 0.8, 5));
    accept(std::make_unique<Bicycle>("Stels Navigator", 8));
    accept(std::make_unique<Helmet>("Шлем M"));
    accept(std::make_unique<DockingStation>("Док-станция у корпуса", 0.8));
    accept(std::make_unique<ChargingCabinet>("Зарядный шкаф №1", 1.5));
    std::cout << "\n-- Отказы на техосмотре --\n";
    accept(std::make_unique<ElectricScooter>("Старый Kugoo (перерасход)", 3.5, 6));
    accept(std::make_unique<Bicycle>("Шоссейный Merida (слишком сложный)", 1));
    std::cout << "\n-- Некорректные данные --\n";
    try {
        accept(std::make_unique<ElectricScooter>("Самокат с простотой 15", 0.4, 15));
    } catch (const std::invalid_argument& e) {
        std::cout << "Ошибка: " << e.what() << '\n';
    }
    show_report();
    show_beginners();
    show_sheet();
}