#include "shader.h"

#include <sstream>
#include <fstream>
#include <QFile>

#include <QOpenGLShader>
#include <QOpenGLExtraFunctions>

GLuint loadShaders(QOpenGLExtraFunctions *context ,std::string VertexShaderCode, std::string FragmentShaderCode){

    // Create the shaders
    GLuint VertexShaderID = context->glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = context->glCreateShader(GL_FRAGMENT_SHADER);


    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    context->glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    context->glCompileShader(VertexShaderID);

    // Check Vertex Shader
    context->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    context->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        context->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    context->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    context->glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    context->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    context->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        context->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    GLuint ProgramID = context->glCreateProgram();
    context->glAttachShader(ProgramID, VertexShaderID);
    context->glAttachShader(ProgramID, FragmentShaderID);
    // Bind attribute locations to match the VAO setup in Mesh::synchronize()
    // position -> 0, uv -> 1, normal -> 2, tangent -> 3, bitangent -> 4
    context->glBindAttribLocation(ProgramID, 0, "vertex");
    context->glBindAttribLocation(ProgramID, 1, "uv");
    context->glBindAttribLocation(ProgramID, 2, "normal");
    context->glBindAttribLocation(ProgramID, 3, "tangent");
    context->glBindAttribLocation(ProgramID, 4, "bitangent");
    context->glLinkProgram(ProgramID);

    // Check the program
    context->glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    context->glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        context->glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    context->glDetachShader(ProgramID, VertexShaderID);
    context->glDetachShader(ProgramID, FragmentShaderID);

    context->glDeleteShader(VertexShaderID);
    context->glDeleteShader(FragmentShaderID);



    return ProgramID;
}




GLuint loadShadersFromFileGLSL(QOpenGLExtraFunctions *context, const char * vertex_file_path,const char * fragment_file_path){

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    QString vpath = QString::fromUtf8(vertex_file_path);
    if (vpath.startsWith(":/")) {
        QFile vf(vpath);
        if (!vf.open(QIODevice::ReadOnly)){
            printf("Could not open shader %s.\n", vertex_file_path);
            return 0;
        }
        QByteArray data = vf.readAll();
        VertexShaderCode = std::string(data.constData(), data.size());
        vf.close();
    } else {
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()){
            std::stringstream sstr;
            sstr << VertexShaderStream.rdbuf();
            VertexShaderCode = sstr.str();
            VertexShaderStream.close();
        }else{
            printf("Could not open shader %s.\n", vertex_file_path);
            getchar();
            return 0;
        }
    }
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    QString fpath = QString::fromUtf8(fragment_file_path);
    if (fpath.startsWith(":/")){
        QFile ff(fpath);
        if (ff.open(QIODevice::ReadOnly)){
            QByteArray data = ff.readAll();
            FragmentShaderCode = std::string(data.constData(), data.size());
            ff.close();
        }
    } else {
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::stringstream sstr;
            sstr << FragmentShaderStream.rdbuf();
            FragmentShaderCode = sstr.str();
            FragmentShaderStream.close();
        }
    }

    return loadShaders(context, VertexShaderCode, FragmentShaderCode);

}
