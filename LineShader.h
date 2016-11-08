#ifndef LINESHADER_H
#define LINESHADER_H
#include <QtCore/QResource>
#include <QtQuick/QSGSimpleMaterial>
#include <QtGui/QColor>


struct LineMaterial
{
    QColor color;
    float spread;
    float size;
};

class LineShader : public QSGSimpleMaterialShader<LineMaterial>
{
    QSG_DECLARE_SIMPLE_SHADER(LineShader, LineMaterial)

public:
    const char *vertexShader() const {
        QResource r(":/resources/line.vsh");
        Q_ASSERT(r.isValid());
        return (const char *) r.data();
    }

    const char *fragmentShader() const {
        QResource r(":/resources/line.fsh");
        Q_ASSERT(r.isValid());
        return (const char *) r.data();
    }

    QList<QByteArray> attributes() const {  return QList<QByteArray>() << "pos" << "t"; }

    void updateState(const LineMaterial *m, const LineMaterial *) {
        program()->setUniformValue(id_color, m->color);
        program()->setUniformValue(id_spread, m->spread);
        program()->setUniformValue(id_size, m->size);
    }

    void resolveUniforms() {
        id_spread = program()->uniformLocation("spread");
        id_size = program()->uniformLocation("size");
        id_color = program()->uniformLocation("color");
    }

private:
    int id_color;
    int id_spread;
    int id_size;
};
#endif // LINESHADER_H
