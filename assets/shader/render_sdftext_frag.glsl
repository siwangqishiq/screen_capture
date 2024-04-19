precision mediump float;
precision mediump sampler2DArray;

out vec4 outColor;

in vec3 vUvw;
// uniform sampler2DArray fontTexture;
// uniform vec4 textColor;

void main(){
    // vec4 originColor = texture(fontTexture , vUvw.xyz);
    // float colorMask = originColor.r;
    // outColor = colorMask * textColor;
    outColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
}
