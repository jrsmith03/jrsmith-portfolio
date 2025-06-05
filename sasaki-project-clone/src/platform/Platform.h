#ifndef PLATFORM_H
#define PLATFORM_H

#include "godot_cpp/classes/static_body3d.hpp"
#include "godot_cpp/classes/area3d.hpp"

namespace godot {

class Platform : public StaticBody3D {
	GDCLASS(Platform, StaticBody3D)

public:
	Platform();
	~Platform();

	void _ready() override;
	void _process(double delta) override;
	void on_wallhang_enter(Area3D *area);
	void on_wallhang_exit(Area3D *area);
	Node* player;
	bool area_occupied;
	bool must_update_hang_state;
	

protected:
	static void _bind_methods();

private:
	Area3D* wall_hang_area;

};

}
#endif
