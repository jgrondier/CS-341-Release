// glew must be before glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// contains helper functions such as shader compiler
#include "icg_helper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "grid/grid.h"
#include "screenquad/screenquad.h"
#include "water/water.h"

#include "reflexion_framebuffer.h"
#include "framebuffer.h"
#include "cube/cube.h"
#include "bezier.h"

Grid grid;
Water water;
Cube cube;

int window_width = 1280;
int window_height = 720;

FrameBuffer framebuffer; // here we will generate the heightmap

ReflexionFrameBuffer water_framebuffer;

ReflexionFrameBuffer ref_fb;

ScreenQuad screenquad; // heightmap generator


using namespace glm;

mat4 projection_matrix;
mat4 view_matrix;
mat4 m_view_matrix;

mat4 quad_model_matrix;
mat4 cube_model_matrix;
mat4 reflection_cube_model_martix;

glm::vec3 cam_pos   = glm::vec3(0.0f, 0.0f,  0.3f);
glm::vec3 prev_cam_pos = glm::vec3(0.0f, 0.0f,  0.3f);
glm::vec3 cam_look =  vec3(2.0f, 2.0f, -2.0f);
glm::vec3 cam_up    = glm::vec3(0.0f, 0.0f,  1.0f);

glm::vec2 moved_vec_acc = glm::vec2(0,0); // accumulated movement we have done on xy axes

Bezier cam_path;
Bezier aim_path;
int b_switch = 0;

int cam_mode = 0;

bool keys[1024];
int acc[3] = {0,0,0};
float heights[5] = {0,0,0,0,0};

GLuint framebuffer_texture_id;

void Init(GLFWwindow* window) {

    // sets background color
    glClearColor(0.937, 0.937, 0.937 /*gray*/, 1.0 /*solid*/);

    // enable depth test.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glAlphaFunc(GL_GREATER, 0.1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cube.Init();
    //setup view and projection matrices
    view_matrix = lookAt(cam_pos, cam_look, cam_up);

    m_view_matrix = lookAt(vec3(0,0,-2.0f), cam_look, -cam_up);

    float ratio = window_width / (float) window_height;

    //view_matrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));

    const float h = 1;
    const float h2 = 0.1f;//3.0f;

    projection_matrix = glm::perspective(45.0f, ratio, 0.001f, 1000.0f);
    quad_model_matrix = glm::scale(mat4(1.0f),vec3(h,h,h));
    cube_model_matrix = scale(IDENTITY_MATRIX,vec3(h2,h2,h2));


    glfwGetFramebufferSize(window, &window_width, &window_height);

    const int fb_size = 800;
    framebuffer_texture_id = framebuffer.Init(fb_size, fb_size);
    screenquad.Init();
    grid.Init(framebuffer_texture_id);

    GLuint water_framebuffer_id;
    water_framebuffer_id = water_framebuffer.Init(window_width, window_height);
    GLuint ref_fb_id;
    ref_fb_id = ref_fb.Init(window_width, window_height);

    water.Init(window_width, window_height, water_framebuffer_id, ref_fb_id, framebuffer_texture_id);


}

// gets called for every frame.
void Display() {

    const float time = glfwGetTime();

    view_matrix = lookAt(cam_pos, cam_look, cam_up);

    m_view_matrix = lookAt(vec3(0,0,-2.0f), cam_look, -cam_up);
    vec2 moved_vec = vec2(cam_pos) - vec2(prev_cam_pos);
    quad_model_matrix = translate(quad_model_matrix, vec3(moved_vec,0));
    cube_model_matrix = translate(quad_model_matrix, vec3(0,0,-1));
    moved_vec_acc += moved_vec;


    framebuffer.Clear();
    framebuffer.Bind();
         screenquad.Draw(moved_vec_acc.x,moved_vec_acc.y);
     framebuffer.Unbind();

     water_framebuffer.Clear();
     water_framebuffer.Bind();

     glDisable(GL_BLEND);
     glDisable(GL_DEPTH_TEST);
     glDepthMask(GL_FALSE);
     cube.Draw(cube_model_matrix, view_matrix, projection_matrix, true);
     glDisable(GL_CULL_FACE);
     glEnable(GL_CLIP_PLANE0);
     glEnable(GL_DEPTH_TEST);
     glDepthMask(GL_TRUE);
     grid.Draw(time, quad_model_matrix, view_matrix, projection_matrix, moved_vec_acc, true);
     glDisable(GL_CLIP_PLANE0);
     water_framebuffer.Unbind();

     ref_fb.Clear();
     ref_fb.Bind();
     grid.Draw(time, quad_model_matrix, view_matrix, projection_matrix, moved_vec_acc);
     ref_fb.Unbind();

    glViewport(0,0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.Draw(time, quad_model_matrix, view_matrix, projection_matrix, moved_vec_acc);
    cube.Draw(cube_model_matrix, view_matrix, projection_matrix);
    water.Draw(time, quad_model_matrix, view_matrix, projection_matrix);

    prev_cam_pos = cam_pos;

}
// gets called when the windows is resized.
void ResizeCallback(GLFWwindow* window, int width, int height) {
    window_width = width;
    window_height = height;

    float ratio = window_width / (float) window_height;
    projection_matrix = perspective(45.0f, ratio, 0.001f, 50.0f);

    glViewport(0, 0, window_width, window_height);

    water.UpdateSize(window_width, window_height);

}


void ErrorCallback(int error, const char* description) {
    fputs(description, stderr);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
      keys[key] = true;
    else if(action == GLFW_RELEASE)
      keys[key] = false;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }else if(key == GLFW_KEY_G && action == GLFW_PRESS){
        screenquad.GenerateNoise();
        framebuffer.Clear();
         framebuffer.Bind();
         screenquad.Draw(0,0);
         framebuffer.Unbind();}
    else if(key == GLFW_KEY_C && action == GLFW_PRESS)
        cam_mode = cam_mode == 0? 1:0;
    else if(key == GLFW_KEY_B && action == GLFW_PRESS) //turn on bezier
    {cam_mode = 2;
        cam_path.Start();
        aim_path.Start();
    }
    else if(key == GLFW_KEY_I && action == GLFW_PRESS) // add 1
        b_switch == 0 ? cam_path.Add_point(cam_pos) : aim_path.Add_point(cam_pos);
    else if(key == GLFW_KEY_O && action == GLFW_PRESS) //remove 1
        b_switch == 0 ? cam_path.Remove_point() : aim_path.Remove_point();
    else if(key == GLFW_KEY_P && action == GLFW_PRESS) //clear
        b_switch == 0 ? cam_path.Reset() : aim_path.Reset();
    else if(key == GLFW_KEY_K && action == GLFW_PRESS) // toggle between path and aim
        { if(b_switch == 0)
            {cout << "Bezier aim_path"<<endl; b_switch = 1;}
          else {cout << "Bezier cam_path" << endl; b_switch =0;}}
    else if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {b_switch == 0 ? cam_path.Print() : aim_path.Print();}

}
float c2(float t){ //smooth interpolation funciton
    return 6*pow(t,5) - 15*pow(t,4) + 10*pow(t, 3);
}

void move_sun(){
    if(keys[GLFW_KEY_U])
        grid.SunPos(1);
    if(keys[GLFW_KEY_J])
        grid.SunPos(-1);
}

void do_movement()
{
    GLfloat cameraSpeed = 0.01f;
    GLfloat cameraSpeed2 = 0.02f;
switch (cam_mode){
    case 0:
      cameraSpeed = 0.01f;
      cameraSpeed2 = 0.02f;
    break;
    case 1:
{
      cameraSpeed = 0.002f;
      cameraSpeed2 = 0.05f;
    // to make sure that i get all the param correct
    framebuffer.Bind();
    GLint width,height,internalFormat;
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &internalFormat); // get internal format type of GL texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width); // get width of GL texture
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height); // get height of GL texture
    // just work with RED
    GLint numBytes = 0;
    switch(internalFormat) // determine what type GL texture has...
    {
    case GL_R32F:
    numBytes = width * height * sizeof(GLfloat);
    break;
    default:
        cout << "wrong format of the texture"<< endl;
    }
    GLfloat ps[1];

    glReadPixels((int)(0.5 * width), (int)(0.5*height), 1, 1, GL_RED,
                 GL_FLOAT, ps);

    framebuffer.Unbind();
    float temp = ps[0];
    ps[0] = 0.8*ps[0] + 0.1*heights[3]+0.05*heights[2]+0.025*heights[1]*0.025*heights[0];
    for(int i = 0; i <4; i++)
        heights[i] = heights[i+1];

    heights[4]= ps[0];
    ps[0] = ps[0] > 0 ? ps[0] : ps[0] * 0.5;
    ps[0] = ps[0] < -0.1? -0.1:ps[0];

    if((cam_pos.z = ps[0] + 0.01) < temp)
        cam_pos.z = temp+0.01;
}
break;
case 2: //Bezier
    if(cam_path.Make_step()!= 0)
    {cam_pos = cam_path.Pos();
     cam_look = aim_path.Pos();
    }else
    {
    cam_mode = 0;
    }
}
  // Camera controls
  int acoeff = 2;

  if(keys[GLFW_KEY_UP])
      cam_path.SpeedUp();
  else
  if(keys[GLFW_KEY_DOWN])
      cam_path.SlowDown();

  if(keys[GLFW_KEY_W])
      acc[0] = (acc[0] == 100) ? 100:(acc[0]+ acoeff);
  else
  if(keys[GLFW_KEY_S])
      acc[0] = (acc[0] == -100) ? -100:(acc[0] - acoeff);
  else{
      if (acc[0] > 0) acc[0] -= acoeff;
      if(acc[0] < 0) acc[0] += acoeff;}

  if(keys[GLFW_KEY_D])
      acc[1] = (acc[1] == 100) ? 100:(acc[1]+ acoeff);
  else
  if(keys[GLFW_KEY_A])
      acc[1] = (acc[1] == -100) ? -100:(acc[1] - acoeff);
  else{
      if (acc[1] > 0) acc[1] -= acoeff;
      if(acc[1] < 0) acc[1] += acoeff;}

  if(keys[GLFW_KEY_Q])
      acc[2] = (acc[2] == 100) ? 100:(acc[2]+ acoeff);
  else
  if(keys[GLFW_KEY_E])
      acc[2] = (acc[2] == -100) ? -100:(acc[2] - acoeff);
  else{
      if (acc[2] > 0) acc[2] -= acoeff;
      if(acc[2] < 0) acc[2] += acoeff;}

  float acc2[3] = {acc[0]/100.0f,acc[1]/100.0f,acc[2]/100.0f};
  cam_pos += cameraSpeed *0.25f* normalize(cam_look-cam_pos)*acc2[0]; //forward/backward
  cam_look += cameraSpeed *0.25f* normalize(cam_look-cam_pos)*acc2[0];
  cam_look += cameraSpeed2*cross(normalize(cam_look-cam_pos),cam_up)*acc2[1]; //left/right

  vec3 camera_left = cross(normalize(cam_look-cam_pos),cam_up);
  vec3 camera_up = cross(normalize(cam_look-cam_pos),camera_left);
  cam_look += cameraSpeed2*normalize(camera_up)*acc2[2];//up/down
}


int main(int argc, char *argv[]) {
    // GLFW Initialization
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(ErrorCallback);

    // hint GLFW that we would like an OpenGL 3 context (at least)
    // http://www.glfw.org/faq.html#how-do-i-create-an-opengl-30-context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // attempt to open the window: fails if required version unavailable
    // note some Intel GPUs do not support OpenGL 3.2
    // note update the driver of your graphic card
    GLFWwindow* window = glfwCreateWindow(window_width, window_height,
                                          "RT rendering", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // makes the OpenGL context of window current on the calling thread
    glfwMakeContextCurrent(window);

    // set the callback for escape key
    glfwSetKeyCallback(window, KeyCallback);

    // set the framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, ResizeCallback);


    // GLEW Initialization (must have a context)
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glewExperimental = GL_TRUE; // fixes glew error (see above link)
    if(glewInit() != GLEW_NO_ERROR) {
        fprintf( stderr, "Failed to initialize GLEW\n");
        return EXIT_FAILURE;
    }

    cout << "OpenGL" << glGetString(GL_VERSION) << endl;

    // initialize our OpenGL program
    Init(window);

    // render loop
    while(!glfwWindowShouldClose(window)){
        Display();
        glfwSwapBuffers(window);
        glfwPollEvents();
        do_movement();
        move_sun();
    }

    grid.Cleanup();
    water.Cleanup();
    framebuffer.Cleanup();
    water_framebuffer.Cleanup();
    cube.Cleanup();

    // close OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

