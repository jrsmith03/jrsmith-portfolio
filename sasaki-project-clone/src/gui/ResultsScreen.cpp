#include "ResultsScreen.h"

void ResultsScreen::_bind_methods() {
    ClassDB::bind_method(D_METHOD("show_results", "results_text"), &ResultsScreen::show_results);
}

ResultsScreen::ResultsScreen() : results_label(nullptr) {
}

ResultsScreen::~ResultsScreen() {
}

void ResultsScreen::_ready() {
    // Find the Label node, which should be a child of this Control node
    results_label = get_node<Label>("ResultsLabel");
    if (!results_label) {
        UtilityFunctions::printerr("ResultsLabel node not found in ResultsScreen!");
    }

    // Hide the results screen initially
    set_visible(false);
}

void ResultsScreen::show_results(const String& results_text) {
    // Make the results screen visible
    set_visible(true);

    // Update the results label text, if the label exists
    if (results_label) {
        results_label->set_text(results_text);
    } else {
        UtilityFunctions::printerr("Cannot display results - ResultsLabel is missing!");
    }
}
