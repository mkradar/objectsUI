#ifndef LINEVERTEX_H
#define LINEVERTEX_H
struct LineVertex {
    float x;
    float y;
    float t;
    inline void set(float xx, float yy, float tt) { x = xx; y = yy; t = tt; }
    inline void setXY(float xx, float yy) { x = xx; y = yy; }
};
#endif // LINEVERTEX_H
