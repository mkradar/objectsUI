#ifndef GRAPHQUICKITEM_H
#define GRAPHQUICKITEM_H
#include <QQuickItem>
#include "GraphNode.h"

class GraphQuickItem: public QQuickItem
{
    Q_OBJECT
public:
    GraphQuickItem(): m_samplesChanged(false), m_geometryChanged(false)
    {
        setFlag(ItemHasContents, true);
    }

protected:
    QSGNode* updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *data)
    {
        /*QSGGeometryNode *node = static_cast<QSGGeometryNode *>(n);
        if (!node)
        {
            node = new QSGSimpleRectNode(boundingRect(), Qt::white);
        }

        node->removeAllChildNodes();

        qreal msPerScreen = 10000;
        qreal pitchesPerScreen = 128;
        qreal x_factor = (qreal) boundingRect().width() / msPerScreen;
        qreal y_factor = (qreal) boundingRect().height() / pitchesPerScreen;

        for (unsigned int i = 0; i < m_stream->notes.size(); i++)
        {
            shared_ptr<Note> note = m_stream->notes.at(i);
            qreal left = boundingRect().left() + note->getTime() * x_factor;
            qreal top = boundingRect().top() + note->getPitch() * y_factor;
            qreal width;
            qreal height = y_factor;

            if (note->getDuration() != 0)
            {
                width = note->getDuration() * x_factor;
            }
            else
            {
                // TODO
                width = 500 * x_factor;

            }

            QRectF noteRectangle = QRectF(left, top, width, height);
            node->appendChildNode(new QSGSimpleRectNode(noteRectangle, Qt::black));
        }
        node->markDirty(QSGNode::DirtyForceUpdate);
        return node;*/

        /* if(oldNode == NULL)
        {
            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
            geometry->setDrawingMode(GL_LINES);
            geometry->setLineWidth(3);
            geometry->vertexDataAsPoint2D()[0].set(0, 0);
            geometry->vertexDataAsPoint2D()[1].set(width(), height());

            QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
            material->setColor(QColor(255, 0, 0));

            QSGGeometryNode *node = new QSGGeometryNode;
            node->setGeometry(geometry);
            node->setFlag(QSGNode::OwnsGeometry);
            node->setMaterial(material);
            node->setFlag(QSGNode::OwnsMaterial);
            return node;
        }

        QSGGeometryNode* node = (QSGGeometryNode*) oldNode;
        node->activeMaterial()->setColor(QColor(0, 255, 0));
        return oldNode; */
        GraphNode *n= static_cast<GraphNode *>(oldNode);

        QRectF rect = boundingRect();

        if (rect.isEmpty()) {
            delete n;
            return 0;
        }

        if (!n) {
            n = new GraphNode();

            // n->background = new NoisyNode(window());
            n->grid = new GridNode();
            n->line = new LineNode(10, 0.5, QColor("steelblue"));
            n->shadow = new LineNode(20, 0.2, QColor::fromRgbF(0.2, 0.2, 0.2, 0.4));

            // n->appendChildNode(n->background);
            n->appendChildNode(n->grid);
            n->appendChildNode(n->shadow);
            n->appendChildNode(n->line);
        }

        if (m_geometryChanged) {
            // n->background->setRect(rect);
            n->grid->setRect(rect);
        }

        if (m_geometryChanged || m_samplesChanged) {
            n->line->updateGeometry(rect, m_samples);
            // We don't need to calculate the geometry twice, so just steal it from the other one...
            n->shadow->setGeometry(n->line->geometry());
        }

        m_geometryChanged = false;
        m_samplesChanged = false;

        return n;
    }

    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
    {
        m_geometryChanged = true;
        update();
        QQuickItem::geometryChanged(newGeometry, oldGeometry);
    }
public slots:
    void appendSample(qreal value)
    {
        m_samples << value;
        m_samplesChanged = true;
        update();
    }

    void removeFirstSample()
    {
        m_samples.removeFirst();
        m_samplesChanged = true;
        update();
    }

private:
    QList<qreal> m_samples;

    bool m_samplesChanged;
    bool m_geometryChanged;
};

#endif // GRAPHQUICKITEM_H
