#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "octpipeline.h"
#include "octconfigfile.h"

class OCTBackend : public QObject
{
    Q_OBJECT

    OCTConfigFile m_octcf;
    OCTConfig m_octc;

    OCTPipeline m_OCTPipeline;
public:
    OCTBackend();

    OCTPipeline* getOCTPipeline() { return &m_OCTPipeline; }

public slots:
    void loadOCT(string path);

    void record();
};

#endif // MAINBACKEND_H
