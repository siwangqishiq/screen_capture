layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aStartPos;
layout(location = 2) in vec2 aEndPos;

uniform mat3 transMat;

out vec2 startPoint;
out vec2 endPoint;

void main(){
    vec3 pos = transMat * vec3(aPos.xy , 1.0f);
    gl_Position = vec4(pos.xy , aPos.z ,1.0f);

    startPoint = aStartPos;
    endPoint = aEndPos;
}
