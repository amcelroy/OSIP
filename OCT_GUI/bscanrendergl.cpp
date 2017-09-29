#include "bscanrendergl.h"

BScanRenderGL::BScanRenderGL(QObject *parent) : QObject(parent)
{

}

void BScanRenderGL::paint(){
    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glDisable(GL_DEPTH_TEST);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_window->resetOpenGLState();
}
