#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "DisplayPipelineStage/displaypipelinestage.h"
#include "Pipeline/pipelinestage.h"
#include <QObject>
#include <bscanimageprovider.h>

namespace OSIP {

    class OCTDisplayStageNotifier : public QObject{
    Q_OBJECT

    signals:
        void onStateChanged();
    };

    class OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        OCTDisplayStageNotifier qt_Notifier;

        BScanImageProvider* m_bscanImageProvider;
    public:
        OCTDisplayStage();

        OCTDisplayStageNotifier* getNotifier() { return &qt_Notifier; }

        void setImageProvider(BScanImageProvider *p) { m_bscanImageProvider = p; }
    protected:
        void workStage();

        void preStage();

        void postStage();

    };
}

#endif // OCTDISPLAYSTAGE_H
