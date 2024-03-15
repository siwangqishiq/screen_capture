precision highp float;

in vec2 vUv;

out vec4 fragColor;

uniform sampler2D uTex;
uniform float uMosaicSize;
uniform vec2 uTexSize;

vec4 mosaicEffect(){
    vec4 originColor = texture(uTex , vUv);
    originColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);

    // vec2 pos = gl_FragCoord.xy;
    float stepX = 1.0f / (uTexSize.x);
    float stepY = 1.0f / (uTexSize.y);

    float startX = (uMosaicSize / uTexSize.x);
    float startY = (uMosaicSize / uTexSize.y);
    vec3 totalColor = vec3(0.0f ,0.0f ,0.0f);
    float count = 1.0f;
    float i = -startY;
    for(; i<= startY;i += stepY){
        // for(float j = -startX; j <= startX ;j += stepX){
        //     // vec4 color = texture(uTex , vec2(vUv.x , vUv.y));
        //     // count = count + 1.0f;
        //     // totalColor = totalColor + color.rgb;
        // }//end for j
    }//end for i
    return vec4(totalColor.r / count , totalColor.g / count , totalColor.b / count , 1.0f);
}

void main(){
    fragColor = mosaicEffect();
}