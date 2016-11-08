#ifndef GRAPHNODE_H
#define GRAPHNODE_H
#include <QSGGeometryNode>
#include "GridNode.h"
#include "LineNode.h"

class GraphNode : public QSGNode
{
public:
    // NoisyNode *background;
    GridNode *grid;
    LineNode *line;
    LineNode *shadow;
};
#endif // GRAPHNODE_H
