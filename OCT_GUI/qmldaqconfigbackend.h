#ifndef QMLDAQCONFIGBACKEND_H
#define QMLDAQCONFIGBACKEND_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QQmlApplicationEngine>

class QMLDAQConfigBackend : public QObject
{
    Q_OBJECT

public:
    QMLDAQConfigBackend(QObject *parent = 0) {

    }

    ~QMLDAQConfigBackend();

    void linkToEngine(QQmlApplicationEngine *engine);

    static const QString SavePath;

public slots:

    void saveDAQConfig(QVariant DAQConfigJSON);

    QString loadDAQConfig();

};

#endif // QMLDAQCONFIGBACKEND_H
