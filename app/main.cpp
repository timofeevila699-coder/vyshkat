#include <iostream>
#include <iomanip>
#include <memory>
#include "Devices/Bicycle.hpp"
#include "Devices/ChargingCabinet.hpp"
#include "Devices/DockingStation.hpp"
#include "Devices/ElectricBicycle.hpp"
#include "Devices/ElectricScooter.hpp"
#include "Devices/Helmet.hpp"
#include "ServiceCenter/ServiceCenter.hpp"
#include "Park/Park.hpp"
#include "Report/Report.hpp"
#include "Interface/IEasy.hpp"

void print_separator() {
    std::cout << "\n" << std::string(70, '=') << "\n\n";
}

void print_device_info(const IInventory* device) {
    std::cout << "  - " << device->get_name();
    if (device->get_number().has_value()) {
        std::cout << " (№ " << device->get_number().value() << ")";
    }
    std::cout << "\n";
}

int main() {
    std::cout << R"(
     ______  _____ _______ _   _  _     _          _
    |  ____ |_   _|__   __| | | | | |   | |   /\   | |
    | |     __ | |    | |  | |_| | | |__ | |  /  \  | |_
    | |    / _\| |    | |  |  _  | |  __ \| | / /\ \ |  _|
    | |___| (__| |_   | |  | | | | | |  | | |/ /__\ \| |
     \_____\____|___|  |_|  |_| |_| |_|  |_|_/_______ \_|

    "ДоЕДЕМ ДО ДЕДЛАЙНА!"
)" << std::endl;

    // ========================================================================
    // СЦЕНАРИЙ 1: Создание сервис-центра и парка
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 1: Инициализация сервиса\n";
    std::cout << "Создаем сервис-центр с ограничениями:\n";
    std::cout << "  - Максимальное энергопотребление: 150 кВт·ч\n";
    std::cout << "  - Минимальная простота для новичка: 4\n\n";

    auto service_center = std::make_shared<ServiceCenter>(150.0, 4);
    auto park = std::make_unique<Park>(service_center);

    std::cout << "✓ Сервис-центр и парк созданы успешно\n";

    // ========================================================================
    // СЦЕНАРИЙ 2: Добавление различных типов устройств
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 2: Добавление устройств в парк\n\n";

    // Прибыль 1: Электрический самокат
    std::cout << "Попытка добавления: Электрический самокат (50 кВт·ч, простота=8)\n";
    if (park->add(std::make_unique<ElectricScooter>("Электрический самокат", 50.0, 8))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // Прибыль 2: Электрический велосипед
    std::cout << "Попытка добавления: Электрический велосипед (95 кВт·ч, простота=6)\n";
    if (park->add(std::make_unique<ElectricBicycle>("Электрический велосипед", 95.0, 6))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // Прибыль 3: Обычный велосипед
    std::cout << "Попытка добавления: Велосипед (простота=5)\n";
    if (park->add(std::make_unique<Bicycle>("Велосипед", 5))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // Прибыль 4: Шлем
    std::cout << "Попытка добавления: Шлем (инвентаризируемое имущество)\n";
    if (park->add(std::make_unique<Helmet>("Шлем"))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // Прибыль 5: Зарядная станция
    std::cout << "Попытка добавления: Зарядная станция (120 кВт·ч)\n";
    if (park->add(std::make_unique<ChargingCabinet>("Зарядная станция", 120.0))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // Прибыль 6: Доковая станция
    std::cout << "Попытка добавления: Доковая станция (35 кВт·ч)\n";
    if (park->add(std::make_unique<DockingStation>("Доковая станция", 35.0))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром\n";
    }

    // ========================================================================
    // СЦЕНАРИЙ 3: Попытка добавления устройства, превышающего лимиты
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 3: Попытка добавления устройства вне допуска\n\n";

    std::cout << "Попытка добавления: Самокат с избыточным расходом (200 кВт·ч, простота=8)\n";
    if (park->add(std::make_unique<ElectricScooter>("Самокат-монстр", 200.0, 8))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром (превышен лимит энергии)\n";
    }

    std::cout << "\nПопытка добавления: Велик слишком сложный для новичков (простота=2)\n";
    if (park->add(std::make_unique<Bicycle>("Велик-сложный", 2))) {
        std::cout << "  ✓ Принят техосмотром\n";
    } else {
        std::cout << "  ✗ Отклонен техосмотром (слишком сложный)\n";
    }

    // ========================================================================
    // СЦЕНАРИЙ 4: Вывод инвентаризованного имущества
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 4: Полный реестр имущества парка \"ВышКат\"\n\n";

    auto items = park->items();
    std::cout << "Всего единиц в парке: " << items.size() << "\n\n";
    std::cout << "Техника и имущество:\n";
    for (const auto& item : items) {
        print_device_info(item);
    }

    // ========================================================================
    // СЦЕНАРИЙ 5: Аналитика парка
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 5: Аналитика и отчёты\n\n";

    auto report = std::make_unique<Report>(*park);

    std::cout << "Общая статистика парка:\n";
    std::cout << "  Количество единиц техники и имущества: "
              << report->count_devices() << "\n";
    std::cout << "  Суммарное суточное потребление энергии: "
              << std::fixed << std::setprecision(1) << report->sum_energy()
              << " кВт·ч\n";
    std::cout << "  Средний расход на единицу: "
              << std::fixed << std::setprecision(2)
              << (report->count_devices() > 0
                  ? report->sum_energy() / report->count_devices()
                  : 0.0)
              << " кВт·ч\n";

    // ========================================================================
    // СЦЕНАРИЙ 6: Техника для новичков
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 6: Техника, подходящая для новичков (простота >= 6)\n\n";

    auto beginners_list = report->list_beginners();
    std::cout << "Кол-во устройств для новичков: " << beginners_list.size() << "\n\n";

    if (!beginners_list.empty()) {
        std::cout << "Список:\n";
        for (const auto& device : beginners_list) {
            std::cout << "  - " << device->get_name();
            if (device->get_number().has_value()) {
                std::cout << " (№ " << device->get_number().value() << ")";
            }
            if (const auto* easy_device = dynamic_cast<const IEasy*>(device)) {
                std::cout << " [простота: " << easy_device->get_easy() << "/10]";
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "Нет устройств, подходящих для новичков\n";
    }

    // ========================================================================
    // СЦЕНАРИЙ 7: Энергоэффективность
    // ========================================================================
    print_separator();
    std::cout << "СЦЕНАРИЙ 7: Энергоэффективность парка\n\n";

    std::cout << "Энергопотребление по категориям:\n";
    double energy_transport = 0.0;
    double energy_infrastructure = 0.0;
    int transport_count = 0;
    int infrastructure_count = 0;

    for (const auto& item : items) {
        if (dynamic_cast<const IEasy*>(item)) {
            // Это транспорт (имеет поле простоты)
            if (const auto* energy_val = dynamic_cast<const IEnergy*>(item)) {
                energy_transport += energy_val->get_energy();
            }
            transport_count++;
        } else {
            // Это инфраструктура
            if (const auto* energy_val = dynamic_cast<const IEnergy*>(item)) {
                energy_infrastructure += energy_val->get_energy();
            }
            infrastructure_count++;
        }
    }

    std::cout << "  Транспортные средства (помощь пользователям): "
              << std::fixed << std::setprecision(1) << energy_transport
              << " кВт·ч (" << transport_count << " единиц)\n";
    std::cout << "  Инфраструктура (станции, зарядки): "
              << std::fixed << std::setprecision(1) << energy_infrastructure
              << " кВт·ч (" << infrastructure_count << " единиц)\n";

    // ========================================================================
    // ИТОГИ
    // ========================================================================
    print_separator();
    std::cout << "ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА\n";
    std::cout << "Парк \"ВышКат\" успешно демонстрирует все возможности:\n";
    std::cout << "  ✓ Техосмотр устройств перед приемкой\n";
    std::cout << "  ✓ Ведение полного реестра имущества\n";
    std::cout << "  ✓ Отчеты по энергопотреблению\n";
    std::cout << "  ✓ Фильтрация техники для новичков\n";
    std::cout << "  ✓ Управление парком через DI-контейнер\n";
    std::cout << "\n\"Доедем до дедлайна!\"\n";

    print_separator();
    return 0;
}