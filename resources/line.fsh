uniform lowp vec4 color;
uniform lowp float qt_Opacity;
uniform lowp float spread;

varying lowp float vT;

#define PI 3.14159265359

void main(void)
{
    lowp float tt = smoothstep(spread, 1.0, sin(vT * PI));
    gl_FragColor = color * qt_Opacity * tt;
}

