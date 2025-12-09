#include "mesh.h"

// Include standard headers
#include <stdlib.h>

// Qt class includes

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QPair>
#include <QString>
#include <QDataStream>

#include <string>
#include <fstream>

#include "tangentspace.h"
#include "shader.h"


GLuint TRI_GL_TYPE = GL_UNSIGNED_INT; // change with TRI_IDX_TYPE in mesh.h!

Mesh::Mesh(){}

void Mesh::setGlFunctions(QOpenGLExtraFunctions *f){
    gl_funcs = f;
}

void Mesh::synchronize() const {

    if (_synchronized){ // maybe we just want to resync
        unsynchronize();
    }

    gl_funcs->glGenTextures(4 , &mapTextureBedrock);
    for (int i = 0; i < 4; ++i){ // map textures
        gl_funcs->glBindTexture(GL_TEXTURE_2D, (&mapTextureBedrock)[i]);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // TODO : Figure out the VertexArray problems
    gl_funcs->glGenVertexArrays(1, &_VAO);
    gl_funcs->glBindVertexArray(_VAO);

    gl_funcs->glGenBuffers(1, &_VBO);
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _VBO);


    gl_funcs->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), &vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well

    gl_funcs->glGenBuffers(1, &_EBO);
    gl_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    gl_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0] , GL_STATIC_DRAW);


    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    if (uvs.size() > 0){
        gl_funcs->glGenBuffers(1, &_UV);
        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _UV);
        gl_funcs->glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(QVector2D), &uvs[0] , GL_STATIC_DRAW);
    }
    gl_funcs->glGenBuffers(1, &_NORMALS);
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    gl_funcs->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), &normals[0] , GL_STATIC_DRAW);

    if (tangents.size() > 0){
        gl_funcs->glGenBuffers(1, &_TANGENT);
        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _TANGENT);
        gl_funcs->glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(QVector3D), &tangents[0] , GL_STATIC_DRAW);
    }

    if (bitangents.size() > 0){
        gl_funcs->glGenBuffers(1, &_BITANGENT);
        gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _BITANGENT);
        gl_funcs->glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(QVector3D), &bitangents[0] , GL_STATIC_DRAW);
    }

    gl_funcs->glEnableVertexAttribArray(0);

    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    // Positions attribute buffer

    gl_funcs->glVertexAttribPointer(
        0,                  // attribute
        3,    // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,  // stride
        (void*)0            // array buffer offset
        );


    // Uvs attribute buffer
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _UV);
    gl_funcs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    gl_funcs->glEnableVertexAttribArray(1);


    // normals
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    gl_funcs->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    gl_funcs->glEnableVertexAttribArray(2);

    // tangents
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _TANGENT);
    gl_funcs->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    gl_funcs->glEnableVertexAttribArray(3);


    // bitangents
    gl_funcs->glBindBuffer(GL_ARRAY_BUFFER, _BITANGENT);
    gl_funcs->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    gl_funcs->glEnableVertexAttribArray(4);

    // Index buffer
    gl_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    gl_funcs->glBindVertexArray(0);

    _synchronized = true;
}

void Mesh::renderForward(const QMatrix4x4 & vpMatrix, QVector3D fv, const QMatrix4x4 & outside_transform) const{

    if (vertices.empty()) return;
    if (!_synchronized){ // FIXME branch prediction may bottleneck a little here? idk
        synchronize();
    }

    gl_funcs->glBindVertexArray(_VAO);
    gl_funcs->glUseProgram(shaderPID);


    for (int i = 0; i < 4; ++i){
        gl_funcs->glActiveTexture(GL_TEXTURE0 + i);
        gl_funcs->glBindTexture(GL_TEXTURE_2D , (&mapTextureBedrock)[i]);


        gl_funcs->glUniform1i(
            gl_funcs->glGetUniformLocation(shaderPID, "heightmap"),
            i
        );

    }

    // todo just tranfer a struct to the gpu with all interesting data
    GLuint mvpUniformLocation = gl_funcs->glGetUniformLocation(shaderPID, "MVP");
    GLuint camForardLocation = gl_funcs->glGetUniformLocation(shaderPID, "view");
    GLuint modelLocation = gl_funcs->glGetUniformLocation(shaderPID, "Model");

    QMatrix4x4 MVP = vpMatrix * outside_transform * transform; // transform the VP into MVP

    gl_funcs->glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, MVP.constData());

    gl_funcs->glUniform3fv(camForardLocation, 1, &fv[0]);

    gl_funcs->glUniformMatrix4fv(modelLocation, 1, GL_FALSE,  transform.constData());

    gl_funcs->glDrawElements(
        GL_TRIANGLES,      // mode
        triangles.size()*3,    // count
        TRI_GL_TYPE,   // type
        (void*)0           // element array buffer offset
        );

    gl_funcs->glUseProgram(0);
    gl_funcs->glBindVertexArray(0);
}

void Mesh::unsynchronize() const {
    gl_funcs->glDeleteBuffers(1, &_VBO);
    gl_funcs->glDeleteBuffers(1, &_EBO);
    gl_funcs->glDeleteBuffers(1, &_UV);
    gl_funcs->glDeleteBuffers(1, &_NORMALS);
    gl_funcs->glDeleteProgram(shaderPID);
    gl_funcs->glDeleteVertexArrays(1, &_VAO);
    gl_funcs->glDeleteTextures(4, &mapTextureBedrock); // map texture and material index texture

    _synchronized = false;

}

void Mesh::setShader(QString vertex_shader, QString fragment_shader){
    shaderPID = loadShadersFromFileGLSL(gl_funcs, vertex_shader.toStdString().c_str(), fragment_shader.toStdString().c_str());
}

void Mesh::setShaderPid(GLuint pid){
    shaderPID = pid;
}

void Mesh::recomputeNormals () {
    normals.resize(vertices.size());
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i] = QVector3D(0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < triangles.size (); i++) {
        QVector3D e01 = vertices[triangles[i].v[1]] -  vertices[triangles[i].v[0]];
        QVector3D e02 = vertices[triangles[i].v[2]] -  vertices[triangles[i].v[0]];
        QVector3D n = QVector3D::crossProduct(e01, e02);
        n.normalize();
        for (unsigned int j = 0; j < 3; j++)
            normals[triangles[i].v[j]] += n;
    }
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i].normalize();

    recomputeTangents();
}

void Mesh::recomputeTangents(){
    computeTangentBasis(vertices, uvs, normals, tangents, bitangents);
}

Mesh Mesh::gen_tesselatedSquare(int nX, int nY, float sX, float sY){
    Mesh o_mesh;

    for (int u = 0; u<nX; ++u){
        for (int v = 0; v<nY; ++v){

            float px = 1. / float(nX) * u;
            float py = 1. / float(nY) * v;

            o_mesh.vertices.push_back(
                QVector3D(
                    (px) * sX,
                            0.0,
                    (py) * sY

                    )
                );
            o_mesh.uvs.push_back(
                QVector2D(
                    px,
                    py
                    )
                );

        }
    }
    for (int i = 0; i<nY-1; ++i){
        for (int j = 0; j<nX-1; ++j){

            o_mesh.triangles.push_back(
                Triangle(
                    nX * j + i,
                    nX*(j+1) +i,
                    nX*(j+1) +i+1
                    )
                );
            o_mesh.triangles.push_back(
                Triangle(
                    nX * j + i,
                    nX*(j+1) +i+1,
                    nX*(j) +i+1
                    )
                );
        }
    }
    o_mesh.recomputeNormals();
    return o_mesh;
}

void Mesh::updatePlaneHeightmap(GeoContextCPU &context)
{
    context.update(0);

    for (int i = 0; i < 4; ++i){
        float* data = context.maps[i].ptrToData();
        gl_funcs->glActiveTexture(GL_TEXTURE0 + i);
        gl_funcs->glBindTexture(GL_TEXTURE_2D , (&mapTextureBedrock)[i]);
        gl_funcs->glTexImage2D(GL_TEXTURE_2D , 0 , GL_R32F, IMGSIZE, IMGSIZE, 0 , GL_RED , GL_FLOAT , (void*)data);
    }

}

Mesh Mesh::load_mesh_off(std::string filename) {
    Mesh res;

    std::ifstream in (filename.c_str ());
    if (!in){
        std::cout << "ERROR [.OFF LOADER]: can't find or open the file (" << filename << ")" << std::endl;
        return res;
    }
    std::string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    res.vertices.resize (sizeV);
    res.triangles.resize (sizeT);

    for (unsigned int i = 0; i < sizeV; i++){
        in >> res.vertices[i][0];
        in >> res.vertices[i][1];
        in >> res.vertices[i][2];
    }

    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> res.triangles[i].v[j];
    }
    in.close();
    res.recomputeNormals();
    return res;
}


