#version 130

varying vec2 texcoord;

uniform vec2 screenResolution;

uniform sampler2D colorSampler;

void main() {
    gl_FragColor.a = 1.0;

    gl_FragColor.rgb = texture2D(colorSampler, texcoord).rgb;

    vec2 uv = texcoord * 2 - 1;

    if(uv.x <= 0.001 && uv.x >= -0.001 && uv.y <= 0.016 && uv.y >= -0.016) gl_FragColor.rgb = 1.0 - gl_FragColor.rgb;
    else if(uv.x <= 0.016 && uv.x >= -0.016 && uv.y <= 0.001 && uv.y >= -0.001) gl_FragColor.rgb = 1.0 - gl_FragColor.rgb;
}