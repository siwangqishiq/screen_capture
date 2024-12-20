precision highp float;
precision highp sampler2DArray;

out vec4 outColor;

uniform float uFontSize;
uniform vec4 uTextColor;
uniform float uFontWeight;

in vec3 vUvw;
uniform sampler2DArray sdfTexture;

float fontSdf(){
    return 2.0f * texture(sdfTexture , vUvw.xyz).r - 1.0f;
}

void main(){
    float sdfValue = fontSdf();
    float boldness = clamp(uFontWeight - 50.0f, 0.0f , 100.0f);
    sdfValue += boldness * 0.005f;
    float smoothSize = min(0.1f , fwidth(sdfValue));
    float sdfAlpha = uTextColor.a * (smoothstep(-smoothSize, smoothSize, sdfValue));
    outColor = vec4(uTextColor.rgb, sdfAlpha);
}
