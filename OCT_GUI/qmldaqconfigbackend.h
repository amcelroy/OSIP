#ifndef QMLDAQCONFIGBACKEND_H
#define QMLDAQCONFIGBACKEND_H

#include <QObject>
#include <QVariant>

class QMLDAQConfigBackend : public QObject
{
    Q_OBJECT

public:
    QMLDAQConfigBackend(QObject *parent = 0) { }

public slots:

    void saveDAQConfig(QVariant DAQConfigJSON);

};

#endif // QMLDAQCONFIGBACKEND_H
