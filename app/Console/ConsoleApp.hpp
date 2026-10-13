#pragma once
#include <memory>
#include "Interface/IInventory.hpp"
#include "Park/Park.hpp"
#include "Report/Report.hpp"
class ConsoleApp {
public:
    ConsoleApp(Park& park, const Report& report);
    void run();
private:
    void print_menu() const;
    void add_device();
    void show_report() const;
    void show_beginners() const;
    void show_sheet() const;
    void run_demo();
    void accept(std::unique_ptr<IInventory> device);
    Park& park_;
    const Report& report_;
};