#include "mesh.h"
#include <qmath.h>
#include <iostream>
#include <fstream>
#include <QDebug>

Mesh::Mesh()
    : m_count(0)
{
    //    generateCube();
    //    QString filename = QString("/home/e20210007813/Bureau/MasterInfo/M2/App-Interactive/TP1/suzanne.off");
    //    load_mesh_off(filename);
}


void Mesh::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void Mesh::triangle(QVector3D p1, QVector3D p2, QVector3D p3)
{
    QVector3D A = p2 - p1;
    QVector3D B = p3 - p1;
    QVector3D normal = A.crossProduct(A,B);

    add(p1,normal);
    add(p2,normal);
    add(p3,normal);
}


void Mesh::quad(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4)
{
    QVector3D u = p4 - p1;
    QVector3D v = p2 - p1;
    QVector3D normal = QVector3D::crossProduct(u, v).normalized();

    add(p1, normal);
    add(p3, normal);
    add(p2, normal);

    add(p1, normal);
    add(p4, normal);
    add(p3, normal);

}

void Mesh::generateCube()
{
    m_data.resize(2500 * 6);

    QVector3D p1(0.0f, 0.0f, 0.0f);
    QVector3D p2(1.0f, 0.0f, 0.0f);
    QVector3D p3(1.0f, 1.0f, 0.0f);
    QVector3D p4(0.0f, 1.0f, 0.0f);

    QVector3D p5(0.0f, 0.0f, 1.0f);
    QVector3D p6(1.0f, 0.0f, 1.0f);
    QVector3D p7(1.0f, 1.0f, 1.0f);
    QVector3D p8(0.0f, 1.0f, 1.0f);

    // Front
    quad(p1, p2, p3, p4);
    // Back
    quad(p6, p5, p8, p7);
    // Left
    quad(p5, p1, p4, p8);
    // Right
    quad(p2, p6, p7, p3);
    // Top face
    quad(p4, p3, p7, p8);
    // Bottom
    quad(p5, p6, p2, p1);
}

void Mesh::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    QVector3D n = QVector3D::normal(QVector3D(0.0f, 0.0f, -0.1f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);

    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
}

void Mesh::load_mesh_off(QString filename) {
    std::cout << "Loading mesh" << std::endl;
    std::string filename_s = filename.toStdString();
    std::cout << "filename : " << filename_s << std::endl;
    std::ifstream in (filename_s.c_str());

    if (!in){
        std::cout << "ERROR [.OFF LOADER]: can't find or open the file (" << filename_s << ")" << std::endl;
        return;
    }

    std::string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;

    m_data.resize(sizeV * sizeT * 3);

    QVector3D vertices_buffer[sizeV];
    for (unsigned int i = 0; i < sizeV; i++) {
        in >> vertices_buffer[i][0];
        in >> vertices_buffer[i][1];
        in >> vertices_buffer[i][2];
    }

    int s;
    QVector3D triangles[3];
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++) {
            in >> s;
            triangles[j] = vertices_buffer[s];
        }
        triangle(triangles[0], triangles[1], triangles[2]);
    }

    in.close();
}

