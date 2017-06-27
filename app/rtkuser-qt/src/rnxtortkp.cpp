#include <QDir>
#include <QFile>

#include "rnxtortkp.h"

RnxToRtkp::RnxToRtkp() :
    _baseAlt("0.0"),
    _baseLat("0.0"),
    _baseLon("0.0"),
    _basePositionAdded(false),
    _configurationFile("configs.conf"),
    _elevantionMask(15),
    _navFile("output.nav"),
    _obsFile("output.obs"),
    _outputFile("output.pos"),
    _outputPath(""),
    _path("./")
{
}

RnxToRtkp::~RnxToRtkp()
{
}

QString RnxToRtkp::command()
{
    QString basePos;
    if(_basePositionAdded) {
        basePos = QStringLiteral("-l %1 %2 %3").arg(_baseLat, _baseLon, _baseAlt);
    }
    return QStringLiteral("\"%1rnx2rtkp\" %8 -m %9 -k \"%2\" \"%3\" \"%4\" \"%5\" -o \"%6%7.pos\"") \
    .arg(_path, _configurationFile, _inputFile, _obsFile, _navFile, _outputPath, _outputFile, basePos \
    , QString::number(_elevantionMask));
}

void RnxToRtkp::setBasePosition(QString lat, QString lon, QString alt)
{
    _baseLat = lat;
    _baseLon = lon;
    _baseAlt = alt;
    _basePositionAdded = true;
}


bool RnxToRtkp::setBinPath(QString path)
{
    _path = path;
    return !QDir(_path).exists();
}

bool RnxToRtkp::setConfigurationFile(QString file)
{
    _configurationFile = file;
    return QFile(_configurationFile).exists();
}

void RnxToRtkp::setElevationMask(uint mask)
{
    _elevantionMask = mask;
}

bool RnxToRtkp::setInputFile(QString file)
{
    _inputFile = file;
    return QFile(_inputFile).exists();
}

bool RnxToRtkp::setNavFile(QString file)
{
    _navFile = file;
    return QFile(_navFile).exists();
}

bool RnxToRtkp::setObsFile(QString file)
{
    _obsFile = file;
    return QFile(_obsFile).exists();
}

bool RnxToRtkp::setOutputFile(QString file)
{
    _outputFile = file;
    return QFile(_outputFile).exists();
}

bool RnxToRtkp::setOutputPath(QString path)
{
    _outputPath = path;
    return QDir(_outputPath).exists();
}



