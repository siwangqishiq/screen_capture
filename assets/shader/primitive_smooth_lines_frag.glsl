precision highp float;

uniform vec4 uColor;
uniform float uRadius;

in vec2 startPoint;
in vec2 endPoint;

out vec4 fragColor;

float renderRoundConnerLine(vec2 pos){
    vec2 dir = normalize(endPoint - startPoint);
    vec2 upDir = vec2(-dir.y , dir.x);
    mat2 vecMat;

    float distanceFromStart = distance(pos , startPoint);
    float distanceFromEnd = distance(pos , endPoint);

    float aaSize = uRadius / 20.0f;
    // const float aaSize = 0.0f;
    if(distanceFromStart <= distanceFromEnd){
        vecMat = mat2(upDir , vec2(pos - startPoint)); //
        float judgeValue = step(0.0f, determinant(vecMat));
        return judgeValue * smoothstep(distance(pos , startPoint) - aaSize
                ,distance(pos , startPoint), uRadius) 
            + (1.0f - judgeValue);
    }else{
        vecMat = mat2(upDir , vec2(pos - endPoint)); //
        float judgeValue = step(0.0f , determinant(vecMat));
        return (1.0f - judgeValue) * smoothstep(distance(pos , endPoint) - aaSize
                , distance(pos , endPoint),uRadius) 
                + judgeValue;
    }
}

void main(){
    vec2 pos = gl_FragCoord.xy;
    fragColor = vec4(uColor.rgb , uColor.a * renderRoundConnerLine(pos));
}