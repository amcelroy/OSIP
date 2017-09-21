#ifndef BSCANQML_H
#define BSCANQML_H

#include <QObject>
#include <QQuickItem>
#include "bscanrendergl.h"

class BScanQML : public QQuickItem
{
public:
    BScanQML();

private:
    BScanRenderGL* _bscanRenderer;

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

};

#endif // BSCANQML_H
