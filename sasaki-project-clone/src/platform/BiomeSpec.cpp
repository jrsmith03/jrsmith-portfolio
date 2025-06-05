#include "BiomeSpec.h"
using namespace godot;

void BiomeSpec::_bind_methods() {
    BIND_ENUM_CONSTANT(biomes::RAINBOW);
    BIND_ENUM_CONSTANT(biomes::LAVA);
    BIND_ENUM_CONSTANT(biomes::OCEAN);
    BIND_ENUM_CONSTANT(biomes::PLANES);
    BIND_ENUM_CONSTANT(biomes::TUNDRA);

}
// BiomeSpec::BiomeSpec() {

// }


