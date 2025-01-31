#ifndef _FBO_HPP
#define _FBO_HPP
#include "C:/glew-2.1.0/include/GL/glew.h"
#include <gl/GL.h>
#include "vmath.h" 

class FBO 
{
    private:
        GLuint fbo                   = 0; 
        GLuint rbo                   = 0; 
        GLuint fboTexture            = 0;
        GLuint shaderProgramObject   = 0; 
        GLint width                  = 512;
        GLint height                 = 512;
        vmath::mat4 perspectiveProjectionMatrix;

    public:
        FBO(GLuint textureWidth = 512, GLuint textureHeight = 512);

        int createShaderProgram(const GLchar* vert, const GLchar* frag, const GLchar* tess_c=NULL, const GLchar* tess_e=NULL, const GLchar* geo=NULL);

        void render(GLint vao);

        void resize(GLint width, GLint height);

        GLuint getTexture(void);

        void bind(void);

        void unbind(void);

        ~FBO();
};





#endif  // _FBO_HPP