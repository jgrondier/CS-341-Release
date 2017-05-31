#pragma once
#include "icg_helper.h"
#include <time.h>
#include <glm/gtx/rotate_vector.hpp>

class ScreenQuad {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_id_;             // texture ID

        int cell_num_;

    public:
        void Init() {

            // compile the shaders
            program_id_ = icg_helper::LoadShaders("screenquad_vshader.glsl",
                                                  "screenquad_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                                 /*V2*/ +1.0f, -1.0f, 0.0f,
                                                 /*V3*/ -1.0f, +1.0f, 0.0f,
                                                 /*V4*/ +1.0f, +1.0f, 0.0f};
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                             vertex_point, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }
            //vtexcoord
            {
                const GLfloat vertex_point_coord[] = { /*V1*/ 0.0f, 0.0f,
                    /*V2*/ +1.0f, 0.0f,
                    /*V3*/ 0.0f, +1.0f,
                    /*V4*/ +1.0f, 1.0f};
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point_coord),
                             vertex_point_coord, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id1 = glGetAttribLocation(program_id_, "vtexcoord");
                glEnableVertexAttribArray(vertex_point_id1);
                glVertexAttribPointer(vertex_point_id1, 2, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
}


            srand(time(NULL));

            GLfloat seed = rand() % 100 + 1; //for some reasons, generation fails for very big values of the seed

            glUniform1f(glGetUniformLocation(program_id_, "seed"), seed);


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
        }


        void Draw(float x, float y) {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            glUniform1f(glGetUniformLocation(program_id_, "i"), x);
            glUniform1f(glGetUniformLocation(program_id_,"j"),y);


            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);

        }
        void GenerateNoise(){
            glUseProgram(program_id_);

            //random vectors generation - two random values [-1,1] each
            srand(time(NULL));

            GLfloat seed = rand() % 100 + 1; //for some reasons, generation fails for very big values of the seed

            glUniform1f(glGetUniformLocation(program_id_, "seed"), seed);


            glUseProgram(0);
        }
};
