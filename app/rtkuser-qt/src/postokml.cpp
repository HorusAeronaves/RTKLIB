#include <QDir>
#include <QFile>

#include "postokml.h"

PosToKml::PosToKml()
{
}

PosToKml::~PosToKml()
{
}

QString PosToKml::command()
{
    return QStringLiteral("\"%1pos2kml\" -o \"%2%3_events.gpx\" -a -gpx \"%2%3_events.pos\"") \
    .arg(_path, _outputPath, _outputFile);
}

bool PosToKml::setBinPath(QString path)
{
    _path = path;
    return QDir(_path).exists();
}

bool PosToKml::setOutputFile(QString file)
{
    _outputFile = file;
    return !QFile(file).exists();
}

bool PosToKml::setOutputPath(QString path)
{
    _outputPath = path;
    return QDir(_path).exists();
}
