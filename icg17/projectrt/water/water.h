#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

#undef M_PI
#define M_PI 3.14159

struct Light_ {
    glm::vec3 La = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 Ld = glm::vec3(0.9f, 0.9f, 0.9f);
    glm::vec3 Ls = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 light_pos = glm::vec3(-1.0f, 1.0f, 3.0f);

    // pass light properties to the shader
    void Setup(GLuint program_id) {
        glUseProgram(program_id);

        // given in camera space
        GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

        GLuint La_id = glGetUniformLocation(program_id, "La");
        GLuint Ld_id = glGetUniformLocation(program_id, "Ld");
        GLuint Ls_id = glGetUniformLocation(program_id, "Ls");

        glUniform3fv(light_pos_id, ONE, glm::value_ptr(light_pos));
        glUniform3fv(La_id, ONE, glm::value_ptr(La));
        glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
        glUniform3fv(Ls_id, ONE, glm::value_ptr(Ls));
    }
};

class Water: public Light_ {

private:
    GLuint vertex_array_id_;                // vertex array object
    GLuint vertex_buffer_object_position_;  // memory buffer for positions
    GLuint vertex_buffer_object_index_;     // memory buffer for indices
    GLuint program_id_;                     // GLSL shader program ID
    GLuint texture_id_;                     // texture ID
    GLuint refraction_texture_id_;                    // mountains texture id
    GLuint h_texture_id_;
    GLuint num_indices_;                    // number of vertices to render
    GLuint MVP_id_;                         // model, view, proj matrix ID

    float water_width_;
    float water_height_;


public:
    void Init(float water_width, float water_height, GLuint texture, GLuint texture1, GLuint texture2) {
        // compile the shaders.
        program_id_ = icg_helper::LoadShaders("water_vshader.glsl",
                                              "water_fshader.glsl");

        this->water_height_ = water_height;
        this->water_width_ = water_width;



        if(!program_id_) {
            exit(EXIT_FAILURE);
        }

        glUseProgram(program_id_);

        // vertex one vertex array
        glGenVertexArrays(1, &vertex_array_id_);
        glBindVertexArray(vertex_array_id_);

        // vertex coordinates and indices
        {
            std::vector<GLfloat> vertices;
            std::vector<GLuint> indices;
            // triangle water with dimension 512x512.
            // always two subsequent entries in 'vertices' form a 2D vertex position.
            int water_dim = 1024;

            for(int i = -1*water_dim; i<= water_dim; i+=2)
            {
                for(int j = -1*water_dim; j<= water_dim; j+=2)
                {
                    vertices.push_back((float)i/water_dim);
                    vertices.push_back((float)j/water_dim);
                }
            }
            bool flag = false;
            for(int i = 0; i <= (water_dim-1)*(water_dim+1); i+= water_dim+1)
            {
                if(flag == false)
                {
                    for(int j = 0; j<=water_dim; j++)
                    {
                        indices.push_back(i+j);
                        indices.push_back(water_dim+1 + j + i);
                    }
                }
                else
                {
                    for(int j = 0; j<=water_dim; j++)
                    {
                        indices.push_back(i+water_dim-j);
                        indices.push_back(i+2*water_dim + 1-j);
                    }
                }
                flag == true?flag =false:flag =true;
            }

            num_indices_ = indices.size();

            // position buffer
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                         &vertices[0], GL_STATIC_DRAW);

            // vertex indices
            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);

            // position shader attribute
            GLuint loc_position = glGetAttribLocation(program_id_, "position");
            glEnableVertexAttribArray(loc_position);
            glVertexAttribPointer(loc_position, 2, GL_FLOAT, DONT_NORMALIZE,
                                  ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        }

        // texture coordinates
        {
            const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                           /*V2*/ 1.0f, 0.0f,
                                                           /*V3*/ 0.0f, 1.0f,
                                                           /*V4*/ 1.0f, 1.0f};

            // buffer
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                         vertex_texture_coordinates, GL_STATIC_DRAW);

            // attribute
            GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                 "vtexcoord");
            glEnableVertexAttribArray(vertex_texture_coord_id);
            glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                  DONT_NORMALIZE, ZERO_STRIDE,
                                  ZERO_BUFFER_OFFSET);
        }


        // Assign texture
        this->texture_id_ = texture;

        GLuint tex_id = glGetUniformLocation(program_id_, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

        this->refraction_texture_id_ = texture1;

        GLuint tex_id1 = glGetUniformLocation(program_id_, "tex1");
        glUniform1i(tex_id1, 1 /*GL_TEXTURE1*/);

        this->h_texture_id_ = texture2;

        GLuint tex_id2 = glGetUniformLocation(program_id_, "tex2");
        glUniform1i(tex_id2, 1 /*GL_TEXTURE2*/);


        // other uniforms
        MVP_id_ = glGetUniformLocation(program_id_, "MVP");

        // to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void Cleanup() {
        glBindVertexArray(0);
        glUseProgram(0);
        glDeleteBuffers(1, &vertex_buffer_object_position_);
        glDeleteBuffers(1, &vertex_buffer_object_index_);
        glDeleteVertexArrays(1, &vertex_array_id_);
        glDeleteProgram(program_id_);
        glDeleteTextures(1, &texture_id_);
        glDeleteTextures(1,&refraction_texture_id_);
        glDeleteTextures(1,&h_texture_id_);
    }
    void UpdateSize(int screenquad_width, int screenquad_height) {
        this->water_width_ = screenquad_width;
        this->water_height_ = screenquad_height;
    }

    void Draw(float time,
              const glm::mat4 &model = IDENTITY_MATRIX,
              const glm::mat4 &view = IDENTITY_MATRIX,
              const glm::mat4 &projection = IDENTITY_MATRIX) {
        glUseProgram(program_id_);
        glBindVertexArray(vertex_array_id_);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, refraction_texture_id_);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, h_texture_id_);

        Light_::Setup(program_id_);

        // setup MVP
        glm::mat4 MVP = projection*view*model;
        GLuint MVP_id = glGetUniformLocation(program_id_, "MVP");
        glUniformMatrix4fv(MVP_id, 1, GL_FALSE, value_ptr(MVP));

        //setup other
        GLint model_id = glGetUniformLocation(program_id_,
                                              "model");
        glUniformMatrix4fv(model_id, ONE, DONT_TRANSPOSE, glm::value_ptr(model));
        GLint view_id = glGetUniformLocation(program_id_,
                                             "view");
        glUniformMatrix4fv(view_id, ONE, DONT_TRANSPOSE, glm::value_ptr(view));

        glUniform1f(glGetUniformLocation(program_id_, "time"), time);

        glUniform1i(glGetUniformLocation(program_id_, "water_height"),this->water_height_);
        glUniform1i(glGetUniformLocation(program_id_, "water_width"),this->water_width_);

        glDrawElements(GL_TRIANGLE_STRIP, num_indices_, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

    }
};
