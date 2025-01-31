#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbo.hpp"

extern FILE* gpFILE;


FBO::FBO(GLuint textureWidth, GLuint textureHeight) : fboTexture(0), rbo(0)
{
    width = textureWidth;
    height = textureHeight;
    
    GLint maxRenderBufferSize = 0;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);
    if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
	{
		fprintf(gpFILE, "texture Size Overflow\n");
		exit(EXIT_FAILURE);
	}
	fprintf(gpFILE, "maxRenderBufferSize = %d\n", maxRenderBufferSize);

    glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFILE, "Framebuffer Creation status is Not complte\n");
		exit(EXIT_FAILURE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int FBO::createShaderProgram(const GLchar* vert, const GLchar* frag, const GLchar* tess_c, const GLchar* tess_e, const GLchar* geo)
{
    GLuint tessControllShaderObject = 0;
    GLuint tessEvaluationShaderObject = 0;
    GLuint geometryShaderObject = 0;

    GLchar* vertexShader = new char[strlen(vert)+1];
    strcpy_s(vertexShader, strlen(vert) + 1, vert);

    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShader, NULL);

	glCompileShader(vertexShaderObject);
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Vertex Shader Compilation Error Log:%s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		delete this;
        return(-1);
	}


    GLchar* fragmentShader = new char[strlen(frag)+1];
    strcpy_s(fragmentShader, strlen(frag) + 1, frag);

    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShader, NULL);

	glCompileShader(fragmentShaderObject);
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Fragment Shader Compilation Error Log:%s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		delete this;
        return(-2);
	}

    if(tess_c != NULL)
    {
        GLchar* tessControllShader = new char[strlen(tess_c)+1];
        strcpy_s(tessControllShader, strlen(tess_c) + 1, tess_c);

        tessControllShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControllShaderObject, 1, (const GLchar**)&tessControllShader, NULL);

        glCompileShader(tessControllShaderObject);
        GLint status = 0;
        GLint infoLogLength = 0;
        GLchar* szInfoLog = NULL;
        glGetShaderiv(tessControllShaderObject, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetShaderiv(tessControllShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                szInfoLog = (GLchar*)malloc(infoLogLength);
                if (szInfoLog != NULL)
                {
                    glGetShaderInfoLog(tessControllShaderObject, infoLogLength, NULL, szInfoLog);
                    fprintf(gpFILE, "tessControll Shader Compilation Error Log:%s\n", szInfoLog);
                    free(szInfoLog);
                    szInfoLog = NULL;
                }
            }
            delete this;
            return(-3);
        }
    }

    if(tess_e != NULL)
    {
        GLchar* tessEvaluationShader = new char[strlen(tess_e)+1];
        strcpy_s(tessEvaluationShader, strlen(tess_e) + 1, tess_e);

        tessEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEvaluationShaderObject, 1, (const GLchar**)&tessEvaluationShader, NULL);

        glCompileShader(tessEvaluationShaderObject);
        GLint status = 0;
        GLint infoLogLength = 0;
        GLchar* szInfoLog = NULL;
        glGetShaderiv(tessEvaluationShaderObject, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetShaderiv(tessEvaluationShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                szInfoLog = (GLchar*)malloc(infoLogLength);
                if (szInfoLog != NULL)
                {
                    glGetShaderInfoLog(tessEvaluationShaderObject, infoLogLength, NULL, szInfoLog);
                    fprintf(gpFILE, "tessEvaluation Shader Compilation Error Log:%s\n", szInfoLog);
                    free(szInfoLog);
                    szInfoLog = NULL;
                }
            }
            delete this;
            return(-3);
        }
    }
    

    if(geo != NULL)
    {
        GLchar* geometryShader = new char[strlen(geo)+1];
        strcpy_s(geometryShader, strlen(geo) + 1, geo);

        geometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShaderObject, 1, (const GLchar**)&geometryShader, NULL);

        glCompileShader(geometryShaderObject);
        GLint status = 0;
        GLint infoLogLength = 0;
        GLchar* szInfoLog = NULL;
        glGetShaderiv(geometryShaderObject, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetShaderiv(geometryShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0)
            {
                szInfoLog = (GLchar*)malloc(infoLogLength);
                if (szInfoLog != NULL)
                {
                    glGetShaderInfoLog(geometryShaderObject, infoLogLength, NULL, szInfoLog);
                    fprintf(gpFILE, "geometry Shader Compilation Error Log:%s\n", szInfoLog);
                    free(szInfoLog);
                    szInfoLog = NULL;
                }
            }
            delete this;
            return(-3);
        }
    }


	//Shader Program
	shaderProgramObject = glCreateProgram();

	glAttachShader(shaderProgramObject, vertexShaderObject);
    if(tess_c != NULL || tess_e != NULL)
    {
        glAttachShader(shaderProgramObject, tessControllShaderObject);
	    glAttachShader(shaderProgramObject, tessEvaluationShaderObject);
    }
    if(geo != NULL)
	    glAttachShader(shaderProgramObject, geometryShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	glLinkProgram(shaderProgramObject);
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFILE, "Shader Program Linking Error Log:%s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
        
			}
		}
		return(-4);
	}

    return(0);
}


void FBO::render(GLint vao)
{
    bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramObject);

    vmath::mat4 modelMatrix = vmath::mat4::identity();
	vmath::mat4 translationMatrix = vmath::mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
    modelMatrix = translationMatrix;

    vmath::mat4 modelViewProjectionMatrix = vmath::mat4::identity();
    modelViewProjectionMatrix = perspectiveProjectionMatrix*modelMatrix;
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramObject, "uMVPMatrix"), 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

    glUseProgram(0);

    unbind();
}


void FBO::resize(GLint iWidth, GLint iHeight)
{
    glDeleteTextures(1, &fboTexture);
    glDeleteRenderbuffers(1, &rbo);

    GLint maxRenderBufferSize = 0;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);
    if (maxRenderBufferSize < width || maxRenderBufferSize < height)
	{
		fprintf(gpFILE, "texture Size Overflow\n");
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, iWidth, iHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFILE, "Framebuffer Creation status is Not complte\n");
		exit(EXIT_FAILURE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



    if (iHeight <= 0)
	    iHeight = 1;
	
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)iWidth / (GLfloat)iHeight, 0.1f, 100.0f);

	glViewport(0, 0, (GLsizei)iWidth, (GLsizei)iHeight);
}


GLuint FBO::getTexture(void)
{
    return(fboTexture);
}


void FBO::bind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


void FBO::unbind(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FBO::~FBO()
{
    if (fboTexture)
	{
		glDeleteTextures(1, &fboTexture);
		fboTexture = 0;
	}

	if (rbo)
	{
		glDeleteRenderbuffers(1, &rbo);
		rbo = 0;
	}

	if (fbo)
	{
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
	}
}