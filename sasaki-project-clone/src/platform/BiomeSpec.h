#ifndef BIOME_SPEC_H
#define BIOME_SPEC_H


#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <map>
using namespace godot;
class BiomeSpec : public Object {
    GDCLASS(BiomeSpec, Object);
    public:

        enum biomes { 
            RAINBOW , 
            LAVA,
            PLANES,
            OCEAN,
            TUNDRA
        };
        enum objects { 
            COIN , 
            MOB_SHOOTER,
            MOB_KIRBY,
            GLIDER,
            SPEED_BOOST
        };
    
    protected:
        static void _bind_methods();
};

VARIANT_ENUM_CAST(BiomeSpec::biomes);
VARIANT_ENUM_CAST(BiomeSpec::objects);

#endif