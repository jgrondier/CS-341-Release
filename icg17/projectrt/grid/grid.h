#pragma once
#include "icg_helper.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "check_error_gl.h"

struct Light {
        glm::vec3 La = glm::vec3(0.15f, 0.15f, 0.15f);
        glm::vec3 Ld = glm::vec3(0.85f, 0.9f, 1.0f);

        glm::vec3 light_pos = glm::vec3(-1.0f, 0.8f, 1.0f);

        // pass light properties to the shader
        void Setup(GLuint program_id) {
            glUseProgram(program_id);

            // given in camera space
            GLuint light_pos_id = glGetUniformLocation(program_id, "light_pos");

            GLuint La_id = glGetUniformLocation(program_id, "La");
            GLuint Ld_id = glGetUniformLocation(program_id, "Ld");

            glUniform3fv(light_pos_id, ONE, glm::value_ptr(light_pos));
            glUniform3fv(La_id, ONE, glm::value_ptr(La));
            glUniform3fv(Ld_id, ONE, glm::value_ptr(Ld));
        }
};

class Grid: public Light {

    private:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint texture_id_;                     // heightmap texture ID
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID

        GLuint texture_grass_id_;               // grass texture ID
        GLuint texture_rock_id_;
        GLuint texture_sand_id_;
        GLuint texture_snow_id_;
        GLuint* ids[4] = {&texture_grass_id_, &texture_rock_id_, &texture_sand_id_, &texture_snow_id_};


    public:
        void Init(GLuint texture) {
            // compile the shaders.

            program_id_ = icg_helper::LoadShaders("grid_vshader.glsl",
                                                  "grid_fshader.glsl");


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
                // always two subsequent entries in 'vertices' form a 2D vertex position.
                int grid_dim = 1024;

                for(int i = -1*grid_dim; i<= grid_dim; i+=2)
                {
                    for(int j = -1*grid_dim; j<= grid_dim; j+=2)
                    {
                        vertices.push_back((float)i/grid_dim);
                        vertices.push_back((float)j/grid_dim);
                    }
                }
                bool flag = false;
                for(int i = 0; i <= (grid_dim-1)*(grid_dim+1); i+= grid_dim+1)
                {
                    if(flag == false)
                    {
                       for(int j = 0; j<=grid_dim; j++)
                         {
                             indices.push_back(i+j);
                            indices.push_back(grid_dim+1 + j + i);
                         }
                    }
                    else
                    {
                        for(int j = 0; j<=grid_dim; j++)
                        {
                             indices.push_back(i+grid_dim-j);
                             indices.push_back(i+2*grid_dim + 1-j);
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
            string filenames[4] = {"grass.tga", "rock.tga", "sand.tga", "snow1.tga"};
            const GLchar* unames[4] = {"grass_tex","rock_tex", "sand_tex", "snow_tex"};
            // load grass texture
            for (int i=0;i<=3;i++)
            {
                int width;
                int height;
                int nb_component;
                string filename = filenames[i];
                // set stb_image to have the same coordinates as OpenGL
                stbi_set_flip_vertically_on_load(1);
                unsigned char* image = stbi_load(filename.c_str(), &width,
                                                 &height, &nb_component, 0);

                if(image == nullptr) {
                    throw(string("Failed to load texture"));
                }

                glGenTextures(1, ids[i]);
                glBindTexture(GL_TEXTURE_2D, *ids[i]);


                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


                if(nb_component == 3) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                                 GL_RGB, GL_UNSIGNED_BYTE, image);
                } else if(nb_component == 4) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE, image);
                }
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(image);

                glUniform1i(glGetUniformLocation(program_id_, unames[i]), i+1);
            }

            // heightmap texture coordinates
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


            glBindTexture(GL_TEXTURE_2D, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // to avoid the current object being polluted
            // cleanup


            Light::Setup(program_id_);

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
            for (int i = 0; i<=3; i++)
                glDeleteTextures(1, ids[i]);
        }
        void SunPos(int x){
            float speedFactor = 0.02;
            glUseProgram(program_id_);
                Light::light_pos = glm::rotateY(glm::vec3(light_pos),x*speedFactor);

                GLuint light_pos_id = glGetUniformLocation(program_id_, "light_pos");
                glUniform3fv(light_pos_id, ONE, glm::value_ptr(Light::light_pos));
        }

        void Draw(float time,
                    const glm::mat4 &model = IDENTITY_MATRIX,
                  const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX, glm::vec2 pos = glm::vec2(0,0), bool reflection = false) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            // bind textures
             glActiveTexture(GL_TEXTURE0);
             glBindTexture(GL_TEXTURE_2D, texture_id_);
             for (int i = 0; i<=3; i++){
             glActiveTexture(GL_TEXTURE1 + i);
             glBindTexture(GL_TEXTURE_2D, *ids[i]);
             }

            // setup MVP
             glm::mat4 MVP = projection*view*model;
             GLuint MVP_id = glGetUniformLocation(program_id_, "MVP");
             glUniformMatrix4fv(MVP_id, 1, GL_FALSE, value_ptr(MVP));

             //setup other

             glUniform1f(glGetUniformLocation(program_id_, "time"), time);
             glUniform1f(glGetUniformLocation(program_id_, "i1"), pos.x);
             glUniform1f(glGetUniformLocation(program_id_, "j1"), pos.y);

             glUniform1f(glGetUniformLocation(program_id_, "is_reflection"), reflection);


            // draw

            glDrawElements(GL_TRIANGLE_STRIP, num_indices_, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
