#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "octpipeline.h"
#include "octconfigfile.h"

class mainbackend : public QObject
{
    Q_OBJECT

    OCTConfigFile m_octcf;
    OCTConfig m_octc;

    OCTPipeline m_OCTPipeline;
public:
    mainbackend();

    OCTPipeline* getOCTPipeline() { return &m_OCTPipeline; }

public slots:
    void loadOCT(string path);


};

#endif // MAINBACKEND_H
