#pragma once

#include <QObject>

class QString;

class PosToKml : public QObject
{
    Q_OBJECT
public:
    PosToKml();
    ~PosToKml();

    QString command();
    bool setBinPath(QString path);
    bool setOutputFile(QString file);
    bool setOutputPath(QString path);
private:
    QString _outputFile;
    QString _outputPath;
    QString _path;
};
