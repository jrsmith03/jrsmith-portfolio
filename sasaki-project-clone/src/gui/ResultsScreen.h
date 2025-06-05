#ifndef RESULTSSCREEN_H
#define RESULTSSCREEN_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class ResultsScreen : public Control {
    GDCLASS(ResultsScreen, Control)

private:
    Label* results_label;

protected:
    static void _bind_methods();

public:
    ResultsScreen();
    ~ResultsScreen();

    void _ready() override;
    void show_results(const String& results_text);
};

#endif // RESULTSSCREEN_H
