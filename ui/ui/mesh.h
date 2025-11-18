#ifndef MESH_H
#define MESH_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <fstream>


//std::ifstream& operator >> (std::ifstream& stream, QVector3D& vector){
//    for (unsigned int j = 0; j < 3; j++)
//        stream >> vector[j];
//    return stream;
//}

class Mesh
{
public:
    Mesh();
    ~Mesh() = default;
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    void load_mesh_off(QString filename);

signals :



private:
    void triangle(QVector3D p1, QVector3D p2, QVector3D p3);
    void quad(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v, const QVector3D &n);
    void generateCube();
    void recomputeNormals ();


    QVector<GLfloat> m_data;
    int m_count;
};

#endif // MESH_H
