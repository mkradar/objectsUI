attribute highp vec4 pos;
attribute highp float t;

uniform lowp float size;
uniform highp mat4 qt_Matrix;

varying lowp float vT;

void main(void)
{
    vec4 adjustedPos = pos;
    adjustedPos.y += (t * size);
    gl_Position = qt_Matrix * adjustedPos;

    vT = t;
}

