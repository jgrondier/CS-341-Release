#version 330
uniform sampler2D tex;
uniform sampler2D grass_tex;
uniform sampler2D rock_tex;
uniform sampler2D sand_tex;
uniform sampler2D snow_tex;
uniform bool is_reflection;

in vec2 uv;
in float height;
uniform float i1;
uniform float j1;


out vec4 color;

uniform vec3 La, Ld;
uniform vec3 light_pos;

void main() {

    // normal calculation

    // x=-1
    float t_01 = textureOffset(tex, uv, ivec2(-1,  0)).r;
    // x=0
    float t_10 = textureOffset(tex, uv, ivec2( 0, -1)).r;
    float t_12 = textureOffset(tex, uv, ivec2( 0, +1)).r;
    // x=+1
    float t_21 = textureOffset(tex, uv, ivec2(+1,  0)).r;

    vec3 dy = normalize(vec3(0,2/800.0, (t_21-t_01)/2));
    vec3 dx = normalize(vec3(2/800.0,0,(t_12-t_10)/2));

    vec3 normal = normalize(cross(dx, dy));

    float dotnl = (dot(normal,normalize(light_pos)) > 0.0f )? dot(normal,normalize(light_pos)) : 0.0f;

    const int mult[4] = int[](20,20,10,20);

    //for the texture positions to move together with camera
    vec2 uv1 = (uv + (vec2(i1,j1)));

    vec3 color_;

    const float sand_level = -0.095;
    const float grass_level = 0.01;
    const float rock_level = 0.09;
    const float snow_level = 0.95;
    if (height <= -0.1) {
        color_ = mix(texture(sand_tex,uv1*mult[0]).rgb, vec3(51/256.0f,65/256.0f,133/256.0f),(abs(height) - 0.1)*30);
    } else if (height <= sand_level){
        color_ = texture(sand_tex,uv1*mult[0]).rgb ;
    }else if(height <= grass_level) {
        color_ = mix(texture(sand_tex,uv1*mult[0]).rgb, texture(grass_tex,uv1*mult[1]).rgb, clamp((height - sand_level)*25,0,1)); // height ranges from -0.1, thus to get good ratio
    } else if(height <= rock_level) {
        color_ = mix(texture(grass_tex,uv1*mult[1]).rgb, texture(rock_tex,uv1*mult[2]).rgb,clamp((height - grass_level)*20,0,1));
    } else {
        color_ = mix(texture(rock_tex,uv1*mult[2]).rgb, texture(snow_tex,uv1*mult[3]).rgb,clamp(2*(height - rock_level)*4,0,1));
    }

    color_ = color_*dotnl*Ld*5/6 + color_*La*5;

    color = vec4(color_, 1.0f);

}
