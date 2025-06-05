#include "Coin.h"

using namespace godot;

void Coin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_coin_collect", "body"), &Coin::on_coin_collect);

    ADD_SIGNAL(MethodInfo("update_score", PropertyInfo(Variant::INT, "new_score")));
}

Coin::Coin() {
}

Coin::~Coin() {
}

void Coin::_ready() {
    state = Engine::get_singleton();
    value = COIN_VALUE;
    coin_area = get_node<Area3D>("./Area3D");
    if (coin_area) {
        coin_area->connect("body_entered", Callable(this, "on_coin_collect"));
    } 
}
void Coin::_process(double delta) {
    float change = 0.1;
    if (this->get_rotation().y >= 360) {
        change = -change;
    }
    this->set_rotation(get_rotation() + Vector3(0, change, 0));
}


void Coin::on_coin_collect(Node3D* body) {
    UtilityFunctions::print("body that picked the coin: ", body->get_name());
    String to_compare = "Player";
    if (body->get_name() == to_compare)
    {
        body->call("on_update_score", value);
        // Player* thisPlayer = static_cast<Player*>(body);
        // if (thisPlayer)
        // {   
        //     UtilityFunctions::print("call the player callback here");
        //     thisPlayer->on_update_score(1);
        // } else {
        //     UtilityFunctions::print("failed to cast body to Player");
        // }
    }
    emit_signal("update_score", 1);
    this->queue_free();
}
