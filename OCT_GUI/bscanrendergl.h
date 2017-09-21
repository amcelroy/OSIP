#ifndef BSCANRENDERGL_H
#define BSCANRENDERGL_H

#include <QObject>
#include <QWidget>
#include <QOpenGLFunctions>
#include <QQuickWindow>

class BScanRenderGL : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit BScanRenderGL(QObject *parent = nullptr);

    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

signals:

public slots:
    void paint();

private:
    QSize m_viewportSize;
    QQuickWindow *m_window;
};

#endif // BSCANRENDERGL_H
