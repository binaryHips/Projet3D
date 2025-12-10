#ifndef TANGENTSPACE_H
#define TANGENTSPACE_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

void computeTangentBasis(
    // inputs
    QVector<QVector3D> & vertices,
    QVector<QVector2D> & uvs,
    QVector<QVector3D> & normals,
    // outputs
    QVector<QVector3D> & tangents,
    QVector<QVector3D> & bitangents
    );

#endif // TANGENTSPACE_H
