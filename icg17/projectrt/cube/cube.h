#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

static const unsigned int NbCubeVertices = 36;
static const glm::vec3 CubeVertices[] =
{
    glm::vec3(-3, -3, -3),
    glm::vec3(-3, 3, -3),
    glm::vec3(3, -3, -3),
    glm::vec3(-3, 3, -3),
    glm::vec3(3, -3, -3),
    glm::vec3(3, 3, -3),
    glm::vec3(3, 3, 3),
    glm::vec3(3, -3, 3),
    glm::vec3(3, 3, -3),
    glm::vec3(3, -3, 3),
    glm::vec3(3, 3, -3),
    glm::vec3(3, -3, -3),
    glm::vec3(3, 3, 3),
    glm::vec3(-3, 3, 3),
    glm::vec3(3, -3, 3),
    glm::vec3(-3, 3, 3),
    glm::vec3(3, -3, 3),
    glm::vec3(-3, -3, 3),
    glm::vec3(-3, -3, 3),
    glm::vec3(-3, -3, -3),
    glm::vec3(3, -3, 3),
    glm::vec3(-3, -3, -3),
    glm::vec3(3, -3, 3),
    glm::vec3(3, -3, -3),
    glm::vec3(-3, 3, -3),
    glm::vec3(-3, -3, -3),
    glm::vec3(-3, 3, 3),
    glm::vec3(-3, -3, -3),
    glm::vec3(-3, 3, 3),
    glm::vec3(-3, -3, 3),
    glm::vec3(3, 3, -3),
    glm::vec3(-3, 3, -3),
    glm::vec3(3, 3, 3),
    glm::vec3(-3, 3, -3),
    glm::vec3(3, 3, 3),
    glm::vec3(-3, 3, 3)
};


static const unsigned int NbCubeUVs = 36;

static const glm::vec2 CubeUVs[] =
{


    glm::vec2(0.499, 0.333),
    glm::vec2(0.499, 0.001),
    glm::vec2(0.251, 0.333),
    glm::vec2(0.499, 0.001),
    glm::vec2(0.251, 0.333),
    glm::vec2(0.251, 0.001),

    glm::vec2(0.0, 0.665),
    glm::vec2(0.25, 0.665),
    glm::vec2(0.0, 0.334),
    glm::vec2(0.25, 0.665),
    glm::vec2(0.0, 0.334),
    glm::vec2(0.25, 0.334),

    glm::vec2(0.251, 0.999),
    glm::vec2(0.499, 0.999),
    glm::vec2(0.251, 0.666),
    glm::vec2(0.499, 0.999),
    glm::vec2(0.251, 0.666),
    glm::vec2(0.499, 0.666),


    glm::vec2(0.5, 0.665),
    glm::vec2(0.5, 0.334),
    glm::vec2(0.25, 0.665),
    glm::vec2(0.5, 0.334),
    glm::vec2(0.25, 0.665),
    glm::vec2(0.25, 0.334),



    glm::vec2(0.75, 0.334),
    glm::vec2(0.5, 0.334),
    glm::vec2(0.75, 0.666),
    glm::vec2(0.5, 0.334),
    glm::vec2(0.75, 0.666),
    glm::vec2(0.5, 0.666),

    glm::vec2(1.0, 0.334),
    glm::vec2(0.75, 0.334),
    glm::vec2(1.0, 0.665),
    glm::vec2(0.75, 0.334),
    glm::vec2(1.0, 0.665),
    glm::vec2(0.75, 0.665)



};

class Cube {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_id_;             // texture ID

    public:
        void Init() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("cube_vshader.glsl",
                                                  "cube_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, NbCubeVertices * sizeof(glm::vec3),
                             &CubeVertices[0], GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // texture coordinates
            {
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, NbCubeUVs * sizeof(glm::vec2),
                             &CubeUVs[0], GL_STATIC_DRAW);

                // attribute
                GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                     "vtexcoord");
                glEnableVertexAttribArray(vertex_texture_coord_id);
                glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                      DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // load texture
            {
                int width;
                int height;
                int nb_component;
                string texture_filename = "cube_texture.tga";
                stbi_set_flip_vertically_on_load(1);
                unsigned char* image = stbi_load(texture_filename.c_str(),
                                                 &width, &height, &nb_component, 0);

                if(image == nullptr) {
                    throw(std::string("Failed to load texture"));
                }

                glGenTextures(1, &texture_id_);
                glBindTexture(GL_TEXTURE_2D, texture_id_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                if(nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                                 GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if(nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image);
                }

                GLuint tex_id = glGetUniformLocation(program_id_, "tex");
                glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

                // cleanup
                glBindTexture(GL_TEXTURE_2D, 0);
                stbi_image_free(image);

            }

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_id_);
        }

        void Draw(const glm::mat4& model,
                  const glm::mat4& view,
                  const glm::mat4& projection,
                  bool reflection = false) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id_);

            // time
            glUniform1f(glGetUniformLocation(program_id_, "time"), glfwGetTime());
            glUniform1f(glGetUniformLocation(program_id_, "is_reflection"), reflection);


            // setup MVP
            glm::mat4 MVP = projection * view * model;
            GLuint MVP_id = glGetUniformLocation(program_id_, "MVP");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, value_ptr(MVP));

            glUniform1f(glGetUniformLocation(program_id_, "is_reflection"), reflection);

            // draw (notice the difference between this draw call and the one in
            //       the other version of the cube, where we have indices and we
            //       use glDrawElements)
            glDrawArrays(GL_TRIANGLES,0,NbCubeVertices);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
