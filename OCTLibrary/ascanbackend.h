#ifndef ASCANBACKEND_H
#define ASCANBACKEND_H

#include <QObject>
#include <QVector>
#include <vector>
#include "octlibrary_global.h"

using namespace std;

class OCTLIBRARYSHARED_EXPORT AScanBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector<qreal> RawAScan READ getRawAScan NOTIFY rawAScanChanged)

    Q_PROPERTY(QVector<qreal> IntensityAScan READ getIntensityAScan NOTIFY intensityAScanChanged)

    QVector<qreal> m_RawAScan;

    QVector<qreal> m_IntensityAScan;

signals:
    void rawAScanChanged();
    void intensityAScanChanged();

public slots:
    //void updateRawAScan(QAbstractSeries *p);
    //void updateIntAScan(QAbstractSeries *p);

public:
    AScanBackend();

    QVector<qreal> getRawAScan(){
        return m_RawAScan;
    }

    QVector<qreal> getIntensityAScan(){
        return m_IntensityAScan;
    }

    void setRawAScan(vector<qreal> r){
        m_RawAScan = QVector<qreal>::fromStdVector(r);
        emit rawAScanChanged();
    }

    void setIntensityAScan(vector<qreal> r){
        m_IntensityAScan = QVector<qreal>::fromStdVector(r);;
        emit intensityAScanChanged();
    }

};

#endif // ASCANBACKEND_H
