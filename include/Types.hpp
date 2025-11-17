#pragma once

#include "glm/fwd.hpp"
#include <cstdint>

namespace etugl {
    using u8    = uint8_t; 
    using u16   = uint16_t;
    using u32   = uint32_t;
    using u64   = uint64_t;

    using i8    = int8_t; 
    using i16   = int16_t;
    using i32   = int32_t;
    using i64   = int64_t;

    using vec1f = glm::vec1;
    using vec2f = glm::vec2;
    using vec3f = glm::vec3;
    using vec4f = glm::vec4;

    using mat2f = glm::mat2;
    using mat3f = glm::mat3;
    using mat4f = glm::mat4;

    using c16 = char16_t;
    using c32 = char32_t;
}
