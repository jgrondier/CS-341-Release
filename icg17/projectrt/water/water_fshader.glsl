#version 330
uniform sampler2D tex;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform int grid_height;
uniform int grid_width;
uniform float time;

in vec2 uv;
in float height;
in vec3 light_dir;
in vec3 view_dir;
in vec4 vpoint_mvp;
in vec3 colorvsh;
out vec4 color;

//in vec3 light_dir;

uniform vec3 La, Ld, Ls;
uniform vec3 light_pos;

void main() {

    ivec2 text_mirror_2d = textureSize(tex, 0);

    float window_width = text_mirror_2d.x;

    float window_height = text_mirror_2d.y;

    float _u = gl_FragCoord.x / window_width;
    float _v =  gl_FragCoord.y / window_height;

    float t_height = texture(tex2, uv).r;

//
//  color = col;
//
//  color = texture(tex, vec2(_u,_v)).rgba;
//
//  color = mix(color, col, 0.5);

    float scale = 1.0f*(1-t_height);
    float time_scale =1.5f;
    int samples = 20;
    float samples_float = float(samples);


    float xoffset = scale * 0.005*cos(time_scale*time*4.0+200.0*_v);
    //float yoffset = 0.05*(1.0+cos(iGlobalTime*3.0+50.0*uv.y));
    float yoffset = scale * 0.005*cos(time_scale*time*4.0+200.0*_v);
    //color = texture(tex, vec2(_u+xoffset , _v+ yoffset));

    vec4 color_tot = vec4(0.0f);
    vec4 color_tot1 = vec4(0.0f);

    for (float i = 0.0f; i < samples; i++) {
        color_tot += texture(tex, vec2(_u+xoffset*(i/samples_float) , _v+ yoffset*(i/samples_float)));
        color_tot1 += texture(tex1, vec2(_u+xoffset*(i/samples_float) , _v+ yoffset*(i/samples_float)));

    }


    color = mix(color_tot, color_tot1, 0.2) / samples_float;

    color.w = 0.8;

}
