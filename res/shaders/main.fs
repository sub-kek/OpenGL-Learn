#version 330

varying vec2 texcoord;

void main() {
    gl_FragColor = vec4(texcoord, 0.0, 1.0);
}