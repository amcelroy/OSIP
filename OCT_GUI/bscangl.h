#ifndef BSCANGL_H
#define BSCANGL_H

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>
#include <QQuickWindow>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QOpenGLVertexArrayObject>
#include <iostream>
#include <vertex.h>

using namespace std;

class BScanRenderer : public QQuickFramebufferObject::Renderer
{
    shared_ptr<unsigned char> m_frameData;

    vector<unsigned long> m_dimensions;

    GLuint m_texture;

    QSize m_size;

    QQuickWindow* m_win;

    QOpenGLBuffer m_vertBuff;

    QOpenGLVertexArrayObject m_object;

    void _initGeometery();

    QOpenGLShaderProgram m_program;

    QOpenGLDebugLogger m_gl_logger;

    bool m_RunOnce = true;
public:
    BScanRenderer();

    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size);

    void render();

    void synchronize(QQuickFramebufferObject *o);

    QOpenGLDebugLogger* getGLDebugLogger() { return & m_gl_logger; }
};

class BScanGL : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    shared_ptr<unsigned char> getFrameData() { return m_frameData; }

    vector<unsigned long> getDimensions() { return m_dimensions; }

private:
    shared_ptr<unsigned char> m_frameData;
    vector<unsigned long> m_dimensions;

    QQuickFramebufferObject::Renderer *createRenderer() const
    {
        BScanRenderer *b = new BScanRenderer();
        QObject::connect(b->getGLDebugLogger(),
                         &QOpenGLDebugLogger::messageLogged,
                         this,
                         &BScanGL::messageLogged);
        return b;
    }

public slots:
    void renderBScan(shared_ptr<unsigned char> data, vector<unsigned long> dim);
    void messageLogged(const QOpenGLDebugMessage &m);


};

#endif // BSCANGL_H
