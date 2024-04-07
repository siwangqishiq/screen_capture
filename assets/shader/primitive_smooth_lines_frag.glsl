precision highp float;

uniform vec4 uColor;
uniform float uRadius;

in vec2 startPoint;
in vec2 endPoint;

out vec4 fragColor;

float renderRoundConnerLine(vec2 pos){
    if(distance(pos , startPoint) <= uRadius){
        return 1.0f;
    }else if(distance(pos , endPoint) <= uRadius){
        return 1.0f;
    }
    return 1.0f;
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    fragColor = vec4(uColor.rgb , renderRoundConnerLine(pos));
    // fragColor = vec4(0.0f , 1.0f ,0.0f ,1.0f);
}