#ifndef LINENODE_H
#define LINENODE_H
#include <QSGGeometryNode>
#include "LineShader.h"
#include "BezierSpline.h"

static const QSGGeometry::AttributeSet &attributes()
{
    static QSGGeometry::Attribute attr[] = {
        QSGGeometry::Attribute::create(0, 2, GL_FLOAT, true),
        QSGGeometry::Attribute::create(1, 1, GL_FLOAT)
    };
    static QSGGeometry::AttributeSet set = { 2, 3 * sizeof(float), attr };
    return set;
}

class LineNode : public QSGGeometryNode
{
public:
    LineNode(float size, float spread, const QColor &color): m_geometry(attributes(), 0)
    {
        setGeometry(&m_geometry);
        m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);

        QSGSimpleMaterial<LineMaterial> *m = LineShader::createMaterial();
        m->state()->color = color;
        m->state()->size = size;
        m->state()->spread = spread;
        m->setFlag(QSGMaterial::Blending);
        setMaterial(m);
        setFlag(OwnsMaterial);
    }

    void updateGeometry(const QRectF &bounds, const QList<qreal> &samples)
    {
        int size = samples.size();
        m_geometry.allocate(size * 2);

        float x = bounds.x();
        float y = bounds.y();
        float w = bounds.width();
        float h = bounds.height();

        float dx = w / (size - 1);

        LineVertex *v = (LineVertex *) m_geometry.vertexData();

        /*for (int i=0; i<size; ++i) {
            v[i*4+0].set(x + 2*dx * i, y + samples.at(i) * h, 0);
            v[i*4+1].set(x + 2*dx * i, y + samples.at(i) * h, 1);
            if(i < size - 1)
            {
                v[i*4+2].set(x + 2*dx*i + dx, y + (samples.at(i) + samples.at(i+1)) * h/2, 0);
                v[i*4+3].set(x + 2*dx*i + dx, y + (samples.at(i) + samples.at(i+1)) * h/2, 1);
            }
        }*/

        /*for (int i=0; i<size; ++i)
        {
            v[i*2+0].set(x + dx * i, y + h, 0);
            v[i*2+1].set(x + dx * (i + 1), y + samples.at(i) * h, 1);
        }*/

        for (int i=0; i<size; ++i)
        {
            v[i*2+0].set(x + dx * i, y + samples.at(i) * h, 0);
            qreal nextValue = (i == size - 1) ? samples.at(i) : samples.at(i+1);
            v[i*2+1].set(x + dx * i, y + nextValue * h, 1);
        }
            /* v[(i + size)*2+0].set(v[i*2+0].x, v[i*2+0].y, 0);
            v[(i + size)*2+1].set(v[i*2+0].x, v[i*2+0].y, 1);
            v[(i + 2*size)*2+0].set(v[i*2+0].x, v[i*2+0].y, 0);
            v[(i + 2*size)*2+1].set(v[i*2+0].x, v[i*2+0].y, 1);
        }

        BezierSpline::GetCurveControlPoints(v, v + 2*size, v + 4*size, size * 2);*/

        markDirty(QSGNode::DirtyGeometry);
    }

private:
    QSGGeometry m_geometry;

};
#endif // LINENODE_H
