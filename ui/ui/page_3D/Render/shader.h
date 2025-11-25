#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShader>
#include <QOpenGLExtraFunctions>

GLuint loadShadersFromFileGLSL(QOpenGLExtraFunctions *context, const char * vertex_file_path,const char * fragment_file_path);

GLuint loadShaders(QOpenGLExtraFunctions *context, std::string VertexShaderCode, std::string FragmentShaderCode);

#endif // SHADER_H
