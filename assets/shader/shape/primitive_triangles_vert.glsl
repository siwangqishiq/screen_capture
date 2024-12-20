layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

uniform mat3 transMat;
uniform mat4 modelMat;

void main(){
    vec3 pos = transMat * vec3(aPos.xy , 1.0f);
    vColor = aColor;
    gl_Position = vec4(pos.xy , aPos.z ,1.0f);
}
