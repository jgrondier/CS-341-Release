#version 330

in vec2 position;

out vec2 uv;
out float height;

out vec3 light_dir;
out vec3 view_dir;
out vec4 vpoint_mvp;
out vec3 colorvsh;

uniform sampler2D tex;
uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;

uniform vec3 light_pos;
uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // convert the 2D position into 3D positions that all lay in a horizontal
    // plane.

    height = -0.1;



    vec3 pos_3d = vec3(position.x, position.y, height);

 //   light_dir = light_pos;//normalize(light_pos - vec3(view*model * vec4(pos_3d, 1.0)));


    vpoint_mvp = MVP * vec4(pos_3d, 1.0);
    gl_Position = MVP * vec4(pos_3d, 1.0);


}
