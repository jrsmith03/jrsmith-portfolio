#include "LobbyScene.h"

using namespace godot;

void LobbyScene::_bind_methods() {
    ADD_SIGNAL(MethodInfo("player_ready_pressed"));
}

void LobbyScene::_ready() {

}

void LobbyScene::_input(const Ref<InputEvent>& event) {
	Input* i = Input::get_singleton();
	if (i->is_action_pressed("player_ready")) {
		// emit signal to net main
		emit_signal("player_ready_pressed");
	}
}