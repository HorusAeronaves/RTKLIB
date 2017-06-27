#pragma once

#include <QObject>

class QString;

class Convbin : public QObject
{
    Q_OBJECT
public:
    Convbin();
    ~Convbin();

    QString command();
    bool setBinPath(QString path);
    void setDelta(double x, double y, double z);
    void setFrequencyNumber(uint number);
    bool setLogFile(QString file);
    bool setLogFormatType(QString format);
    bool setOutputFile(QString file);
    bool setOutputPath(QString path);
    void setRinexVersion(QString version);
private:
    QString _delta;
    QString _freqNumber;
    QString _logFile;
    QString _logFormat;
    QString _outputFile;
    QString _outputPath;
    QString _path;
    QString _rinexVersion;

    static const QStringList _logFormatList;
};
