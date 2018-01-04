#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "pipeline.hpp"
#include <QObject>
#include <bscanimageprovider.h>
#include <boost/signals2.hpp>
#include "enfaceimageprovider.h"

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

        EnFaceImageProvider* m_enFaceImageProvider;
    public:
        OCTDisplayStage();

        OCTDisplayStageNotifier* getNotifier() { return &qt_Notifier; }

        void setBScanImageProvider(BScanImageProvider *p) { m_bscanImageProvider = p; }

        void setEnFaceImageProvider(EnFaceImageProvider *p) { m_enFaceImageProvider = p; }

    protected:
        void work() override;

        void preStage() override;

        void postStage() override;

    };
}

#endif // OCTDISPLAYSTAGE_H
