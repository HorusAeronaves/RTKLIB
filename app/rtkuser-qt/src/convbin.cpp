#include <QDir>
#include <QFile>

#include "convbin.h"

const QStringList Convbin::_logFormatList{"rtcm2", "rtcm3", "nov", \
    "oem3", "ubx", "ss2", "hemis", "stq", "javad", "nvs", "binex", \
    "rt17", "sbf", "cmr", "rinex"};

Convbin::Convbin() :
    _delta("0/0/0"),
    _freqNumber("1"),
    _logFile("./input.UBX"),
    _logFormat("ubx"),
    _outputFile("output"),
    _path("./"),
    _rinexVersion("3.03")
{
}

Convbin::~Convbin()
{
}

QString Convbin::command()
{
    return QStringLiteral("\"%1convbin\" -hd %8 -f %2 -r %3 -v %4 \"%5\" -d \"%6\" -o \"%7.obs\" -n \"%7.nav\"") \
    .arg(_path, QString(_freqNumber), _logFormat, _rinexVersion, _logFile, _outputPath, _outputFile, _delta);
}

bool Convbin::setBinPath(QString path)
{
    _path = path;
    return QDir(_path).exists();
}

void Convbin::setDelta(double x, double y, double z)
{
    _delta = QStringLiteral("%1/%2/%3").arg(z).arg(x).arg(y);
}

void Convbin::setFrequencyNumber(uint number)
{
    _freqNumber = QString::number(number);
}

bool Convbin::setLogFile(QString file)
{
    _logFile = file;
    return QFile(_logFile).exists();
}

bool Convbin::setLogFormatType(QString format)
{
    _logFormat = format;
    return _logFormatList.contains(format);
}

bool Convbin::setOutputFile(QString file)
{
    _outputFile = file;
    return !QFile(_outputFile).exists();
}

bool Convbin::setOutputPath(QString path)
{
    _outputPath = path;
    return QDir(_path).exists();
}

void Convbin::setRinexVersion(QString version)
{
    _rinexVersion = version;
}


