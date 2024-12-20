#include "ui/color.h"

namespace purple{

    glm::vec4 HexColorToVecValue(uint8_t r,uint8_t g , uint8_t b , uint8_t a){
        const float fullColorValue = 255.0f;
        return glm::vec4(static_cast<float>(r) / fullColorValue,
            static_cast<float>(g) / fullColorValue,
            static_cast<float>(b) / fullColorValue,
            static_cast<float>(a) / fullColorValue);
    }

    glm::vec4 ConverColorValue(Color color){
        glm::vec4 result(0.0f , 0.0f , 0.0f , 0.0f);
        switch(color){
            case Transparent:
                break;
            case White:
                result = HexColorToVecValue(255,255,255,255);
                break;
            case Black:
                result = HexColorToVecValue(0,0,0,255);
                break;
            case Red:
                result = HexColorToVecValue(255,0,0,255);
                break;
            case Green:
                result = HexColorToVecValue(0,255,0,255);
                break;
            case Blue:
                result = HexColorToVecValue(0,0,255,255);
                break;
            case Yellow:
                result = HexColorToVecValue(255,255,0,255);
                break;
            case Purple:
                result = HexColorToVecValue(238,130,238,255);
                break;
            case SkyBlue:
                result = HexColorToVecValue(2,204,255,255);
                break;
            case Gray:
                result = HexColorToVecValue(128,128,128,255);
                break;
            case Silver:
                result = HexColorToVecValue(192,192,192,255);
                break;
            case Pink:
                result = HexColorToVecValue(255,192,203,255);
                break;
        }//end switch
        return result;
    }
}
