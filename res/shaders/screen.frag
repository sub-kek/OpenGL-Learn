#version 130
#define GAUSSIAN_SAMPLES 12
#define GAUSSIAN_SIGMA float(GAUSSIAN_SAMPLES) * 0.25

varying vec2 texcoord;

uniform vec2 screenResolution;

uniform sampler2D colorSampler;

void main() {
    gl_FragColor.a = 1.0;

    gl_FragColor.rgb = texture2D(colorSampler, texcoord).rgb;

    vec2 uv = texcoord * 2.0 - 1.0;
    uv.x *= screenResolution.x / screenResolution.y;

    if(uv.x <= 0.001 && uv.x >= -0.001 && uv.y <= 0.01 && uv.y >= -0.01) gl_FragColor.rgb = 1.0 - gl_FragColor.rgb;
    else if(uv.x <= 0.01 && uv.x >= -0.01 && uv.y <= 0.001 && uv.y >= -0.001) gl_FragColor.rgb = 1.0 - gl_FragColor.rgb;
}