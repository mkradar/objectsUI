#ifndef GRIDNODE_H
#define GRIDNODE_H
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>
#include "qmath.h"

#define GRID_SIZE 32

class GridNode : public QSGGeometryNode
{
public:
    GridNode(): m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0)
    {
        setGeometry(&m_geometry);
        m_geometry.setDrawingMode(GL_LINES);

        setMaterial(&m_material);
        m_material.setColor(Qt::gray);
    }

    void setRect(const QRectF &rect)
    {
        int vCount = int((rect.width() - 1) / GRID_SIZE);
        int hCount = int((rect.height() - 1) / GRID_SIZE);

        int lineCount = vCount + hCount;

        QSGGeometry *g = geometry();

        g->allocate(lineCount * 2);

        float x = rect.x();
        float y = rect.y();
        float w = rect.width();
        float h = rect.height();

        QSGGeometry::Point2D *v = g->vertexDataAsPoint2D();

        // Then write the vertical lines
        for (int i=0; i<vCount; ++i)  {
            float dx = (i + 1) * GRID_SIZE;
            v[i*2].set(dx, y);
            v[i*2+1].set(dx, y + h);
        }
        v += vCount * 2;
        // Then write the horizontal lines
        for (int i=0; i<hCount; ++i)  {
            float dy = (i + 1) * GRID_SIZE;
            v[i*2].set(x, dy);
            v[i*2+1].set(x + w, dy);
        }

        // Tell the scenegraph we updated the geometry..
        markDirty(QSGNode::DirtyGeometry);
    }

private:
    QSGFlatColorMaterial m_material;
    QSGGeometry m_geometry;
};
#endif // GRIDNODE_H
