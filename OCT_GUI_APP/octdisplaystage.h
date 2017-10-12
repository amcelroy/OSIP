#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "DisplayPipelineStage/displaypipelinestage.h"
#include "Pipeline/pipelinestage.h"
#include <QObject>

namespace OSIP {

    class OCTDisplayStageNotifier : public QObject{
    Q_OBJECT

    signals:
        void intensityBScanReady(shared_ptr<unsigned char> data, vector<unsigned long> dim);
        void phaseBScanReady(shared_ptr<unsigned char> data, vector<unsigned long> dim);

    };

    class OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        OCTDisplayStageNotifier qt_Notifier;

        unsigned char* RGBA;

    public:
        OCTDisplayStage();

        OCTDisplayStageNotifier* getNotifier() { return &qt_Notifier; }
    protected:
        void workStage();

        void preStage();

        void postStage();


    };
}

#endif // OCTDISPLAYSTAGE_H
