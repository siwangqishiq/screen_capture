precision highp float;

uniform vec4 uColor;

out vec4 fragColor;

void main(){
    fragColor = uColor;
    // fragColor = vec4(0.0f , 1.0f ,0.0f ,1.0f);
}