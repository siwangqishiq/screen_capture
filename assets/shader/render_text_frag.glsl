precision mediump float;
precision mediump sampler2DArray;

in vec3 vUvw;
out vec4 outColor;

uniform sampler2DArray fontTexture;
uniform vec4 textColor;

void main(){
    vec4 originColor = texture(fontTexture , vUvw.xyz);
    float colorMask = originColor.r;
    outColor = vec4(textColor.rgb , textColor.a * colorMask);
//     outColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
}
