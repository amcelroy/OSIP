#include "bscanqml.h"

BScanQML::BScanQML()
{
    connect(this, &QQuickItem::windowChanged, this, &BScanQML::handleWindowChanged);
}

void BScanQML::handleWindowChanged(QQuickWindow *win){
    if(win){
        connect(win, &QQuickWindow::beforeSynchronizing, this, &BScanQML::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &BScanQML::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void BScanQML::sync(){
    if (!_bscanRenderer) {
        _bscanRenderer = new BScanRenderGL();
        connect(window(), &QQuickWindow::beforeRendering, _bscanRenderer, &BScanRenderGL::paint, Qt::DirectConnection);
    }
    _bscanRenderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    _bscanRenderer->setWindow(window());
}

void BScanQML::cleanup()
{
    if (_bscanRenderer) {
        delete _bscanRenderer;
        _bscanRenderer = 0;
    }
}
