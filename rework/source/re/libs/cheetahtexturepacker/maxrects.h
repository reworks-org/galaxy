#ifndef MAXRECTS_H
#define MAXRECTS_H

#include <list>

#include "imagepacker.h"

struct trbl
{
    re::Vector2<int> t, r, b, l;
};

struct MaxRectsNode
{
	re::Rect<int> r; //rect
    //    QSize *i; //image
    trbl b; //border
};
class MaxRects
{
    public:
        MaxRects();
        std::list<MaxRectsNode> F;
		std::list<QRect> R;
		std::list<MaxRectsNode *> FR;
        re::Vector2<int> insertNode(inputImage *);
        int heuristic, w, h, rotation;
        bool leftToRight;
        border_t *border;
};

#endif // MAXRECTS_H
