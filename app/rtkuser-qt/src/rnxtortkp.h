#pragma once

#include <QObject>

class QString;

class RnxToRtkp : public QObject
{
    Q_OBJECT
public:
    RnxToRtkp();
    ~RnxToRtkp();

    QString command();
    void setBasePosition(QString lat, QString lon, QString alt);
    bool setBinPath(QString path);
    bool setConfigurationFile(QString file);
    void setElevationMask(uint mask);
    bool setInputFile(QString file);
    bool setNavFile(QString file);
    bool setObsFile(QString file);
    bool setOutputFile(QString file);
    bool setOutputPath(QString path);

private:
    QString _baseAlt;
    QString _baseLat;
    QString _baseLon;
    bool _basePositionAdded;
    QString _configurationFile;
    uint _elevantionMask;
    QString _inputFile;
    QString _navFile;
    QString _obsFile;
    QString _outputFile;
    QString _outputPath;
    QString _path;
};
