#version 330

in vec2 position;

out vec2 uv;
out float height;

uniform sampler2D tex;
uniform mat4 MVP;
uniform bool is_reflection;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;
    height = texture(tex, uv).r > 0 ? texture(tex, uv).r : texture(tex, uv).r * 0.5;
    vec3 pos_3d = vec3(position.x, position.y, height);
    if (is_reflection) {
        pos_3d = vec3(position.x, position.y, -(height+0.2));
    } else {
        pos_3d = vec3(position.x, position.y, height);
    }
    gl_ClipDistance[0] = height + 0.1;
    gl_Position = MVP * vec4(pos_3d, 1.0);
}
