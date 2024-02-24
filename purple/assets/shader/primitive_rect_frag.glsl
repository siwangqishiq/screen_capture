precision highp float;

uniform vec4 uColor;
uniform int uFillStyle;

out vec4 fragColor;

const int STYLE_FILLED = 0;
const int STYLE_STOKEN = 1;

void main(){
    fragColor = uColor;
}