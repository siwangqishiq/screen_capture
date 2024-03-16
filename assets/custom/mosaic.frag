precision highp float;

in vec2 vUv;

out vec4 fragColor;

uniform sampler2D uTex;
uniform float uMosaicSize;
uniform vec2 uTexSize;

//屏幕坐标转纹理坐标
vec2 toTexUvCoord(vec2 coord){
    return vec2(coord.x / uTexSize.x , coord.y / uTexSize.y);
}

vec4 mosaicEffect(){
    vec4 originColor = texture(uTex , vUv);
    originColor = vec4(1.0f , 0.0f , 0.0f , 1.0f);
    
    vec2 pos = gl_FragCoord.xy; // 当前点坐标
    vec2 uv = toTexUvCoord(pos);
    originColor = texture(uTex , uv);

    float texWidth = uTexSize.x;
    float texHeight = uTexSize.y;

    int idxX = int(pos.x / uMosaicSize);
    int idxY = int(pos.y / uMosaicSize);


    ivec2 startVec = ivec2(int(idxX * uMosaicSize) , int(idxY * uMosaicSize));
    ivec2 endVec = ivec2(min(int((idxX + 1) * uMosaicSize) , int(texWidth)) 
                        , min(int((idxY + 1) * uMosaicSize) , int(texHeight)));

    int count = 0;
    vec3 totalColor = vec3(0.0f , 0.0f , 0.0f);

    for(int i = startVec.y ; i < endVec.y ;i++){
        for(int j = startVec.x ; j< endVec.x ;j++){
            vec2 coord = vec2(float(j) , float(i));
            totalColor += texture(uTex , toTexUvCoord(coord)).rgb;
            count++;
        }
    }
    
    return vec4(totalColor.rgb / float(count) , 1.0f);
}

void main(){
    fragColor = mosaicEffect();
}