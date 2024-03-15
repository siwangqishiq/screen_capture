precision highp float;

in vec2 vUv;

out vec4 fragColor;

uniform sampler2D uTex;
uniform int uMosaicSize;

vec4 mosaicEffect(){
    vec4 originColor = texture(uTex , vUv);
    return vec4(originColor.rrr, 1.0f);
}

void main(){
    fragColor = mosaicEffect();
}