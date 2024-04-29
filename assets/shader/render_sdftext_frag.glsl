precision highp float;
precision highp sampler2DArray;

out vec4 outColor;

uniform float uFontSize;
uniform vec4 uTextColor;

in vec3 vUvw;
uniform sampler2DArray sdfTexture;

float fontSdf(){
    return 2.0f * texture(sdfTexture , vUvw.xyz).r - 1.0f;
}

void main(){
    float sdfValue = fontSdf();
    float SmoothSize = min(0.5f , 8.0f / uFontSize);
    float sdfAlpha = uTextColor.a * (1.0f - smoothstep(0.0f - SmoothSize, 0.0f + SmoothSize, sdfValue));
    outColor = vec4(uTextColor.rgb, sdfAlpha);
}
