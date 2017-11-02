#include "qmldaqconfigbackend.h"

const QString QMLDAQConfigBackend::SavePath = QString("DAQConfig.json");

void QMLDAQConfigBackend::saveDAQConfig(QVariant DAQConfigJSON){
    QString obj = DAQConfigJSON.toString();
    QFile f(SavePath);

    if(f.open(QIODevice::WriteOnly)){
        f.write(obj.toLatin1().constData(), obj.size());
    }
}

QString QMLDAQConfigBackend::loadDAQConfig(){
    QFile f(SavePath);

    if(f.open(QIODevice::ReadOnly)){
        qint64 size = f.size();
        QByteArray qa = f.read(size);
        return QString(qa);
    }else{

    }

    return QString("empty");
}

QMLDAQConfigBackend::~QMLDAQConfigBackend(){
    int x = 0;
}
