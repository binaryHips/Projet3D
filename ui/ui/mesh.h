#pragma once


// #include <array>

// Qt class includes
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QPair>
#include <QString>
#include <QDataStream>

#include <string>

#include "includes_all_cpu.h"
#include "texture.h"



using TRI_IDX_TYPE = unsigned int; // change both if needed!
extern GLuint TRI_GL_TYPE; // defined in mesh.cpp

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (TRI_IDX_TYPE v0, TRI_IDX_TYPE v1, TRI_IDX_TYPE v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    TRI_IDX_TYPE & operator [] (unsigned int iv) { return v[iv]; }
    TRI_IDX_TYPE operator [] (unsigned int iv) const { return v[iv]; }
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    std::array<TRI_IDX_TYPE, 3> v;
};


class Mesh {
protected:

    // QOpenGLFunctions class
    QOpenGLExtraFunctions *gl_funcs; // We use glwidget cuz it inherits Functions and Extrafunctions

    // GPU, VBO etc
    void unsynchronize() const;
    void synchronize() const;

    mutable bool _synchronized = false;
    mutable GLuint _VBO;
    mutable GLuint _VAO; // https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio
    mutable GLuint _EBO;
    mutable GLuint _UV;
    mutable GLuint _NORMALS;

    mutable GLuint _TANGENT;
    mutable GLuint _BITANGENT;


public:

    // Handle<MaterialPBR> material;

    GLuint shaderPID; // public for now.
    QMatrix4x4 transform = QMatrix4x4();    // TODO : FILL THIS WITH 1s
    QVector< QVector3D > vertices;
    QVector< QVector2D > uvs;
    QVector< QVector3D > normals;
    QVector< Triangle > triangles;
    QVector< QVector3D > colors;

    //tangent / bitangent
    QVector<QVector3D> tangents;
    QVector<QVector3D> bitangents;

    QVector< QPair<Texture, QString> > textures; // TODO : Finx Texture definition

    static Mesh gen_tesselatedSquare(int nX, int nY, float sX = 1, float sY = 1);
    static Mesh load_mesh_off(std::string filename);

    // Mesh() = default;
    Mesh();
    ~Mesh()
    {
        if (_synchronized){
            unsynchronize();
        }
    }

    void setGlFunctions(QOpenGLExtraFunctions *f);

    void recomputeNormals();
    void recomputeTangents();

    void setShader(QString vertex_shader, QString fragment_shader);
    void setShaderPid(GLuint pid);

    void renderForward(const QMatrix4x4 & vpMatrix, QVector3D vp, const QMatrix4x4 & outside_transform) const;
    void renderDeferred(const QMatrix4x4 & vpMatrix, QVector3D vp, const QMatrix4x4 & outside_transform, GLuint gShader) const;

    void loadHeightmap(GeoContextCPU context);

    void rotate(float v, QVector3D axis){
        transform.rotate(v, axis);
    }

    // TODO : Figure out where texture is defined
    void addTexture(Texture tex, QString name_in_shader){
        textures.push_back( QPair(tex, name_in_shader));
    }

    // friend std::ostream& operator<< (std::ostream& stream, const Mesh& mesh) {
    //     stream << "Mesh(" << mesh.vertices.size() <<" verts, " << mesh.triangles.size() <<" tris)";
    //     return stream;
    // }

    friend QDataStream& operator<< (QDataStream& stream, const Mesh& mesh) {
        stream << "Mesh(" << mesh.vertices.size() <<" verts, " << mesh.triangles.size() <<" tris)";
        return stream;
    }

    void createDefaultShader();
};
