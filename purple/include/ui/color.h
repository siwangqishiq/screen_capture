#pragma once

#include "glm/vec4.hpp"

namespace purple{
    enum Color{
        Transparent,
        Black,
        White,
        Red,
        Green,
        Blue,
        Yellow,
        Purple,
        SkyBlue,
        Silver,
        Gray,
        Pink
    };

    glm::vec4 ConverColorValue(Color color);

    glm::vec4 HexColorToValue(uint8_t r,uint8_t g , uint8_t b , uint8_t a);
}

