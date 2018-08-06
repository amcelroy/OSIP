#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "pipeline.hpp"
#include <QObject>
#include <QVector>
#include <bscanimageprovider.h>
#include <boost/signals2.hpp>
#include "enfaceimageprovider.h"
#include "octlibrary_global.h"
#include "ascanbackend.h"

namespace OSIP {

    class OCTLIBRARYSHARED_EXPORT OCTDisplayStageNotifier : public QObject{
    Q_OBJECT

    signals:
        void onStateChanged();

    };

    class OCTLIBRARYSHARED_EXPORT OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        OCTDisplayStageNotifier qt_Notifier;

        BScanImageProvider* m_bscanImageProvider;

        EnFaceImageProvider* m_enFaceImageProvider;

        AScanBackend* m_AScanBackend;
    public:
        OCTDisplayStage() { }

        OCTDisplayStageNotifier* getNotifier() { return &qt_Notifier; }

        void setBScanImageProvider(BScanImageProvider *p) { m_bscanImageProvider = p; }

        void setEnFaceImageProvider(EnFaceImageProvider *p) { m_enFaceImageProvider = p; }

        void setAScanBackend(AScanBackend* a){ m_AScanBackend = a; }
    protected:
        void work() override;

        void preStage() override;

        void postStage() override;

    };
}

#endif // OCTDISPLAYSTAGE_H
