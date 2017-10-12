#include "window.h"
#include "ui_window.h"

#include "convbin.h"
#include "postokml.h"
#include "rnxtortkp.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>
#include <QString>
#include <QThread>

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::_MainWindow)
{
    ui->setupUi(this);

    // Add software version
    this->setWindowTitle(this->windowTitle() + " - " + GIT_VERSION);

    // Set mask and default values for base input
    ui->baseLatInput->setValidator(new QDoubleValidator(-90, 90, 12, this));
    ui->baseLonInput->setValidator(new QDoubleValidator(-180, 180, 12, this));
    ui->baseAltInput->setValidator(new QDoubleValidator(8900, -100, 6, this));

    // Make connections and set initial state
    // PPK
    connect(ui->checkConfig, &QCheckBox::stateChanged, [=](int state) {
        ui->configInput->setEnabled(state == Qt::Checked);
        ui->configButton->setEnabled(state == Qt::Checked);
    });
    // IBGE and Base RTK
    connect(ui->checkBaseRtk, &QCheckBox::stateChanged, [=](int state) {
        ui->baseLatInput->setEnabled(state == Qt::Checked);
        ui->baseLonInput->setEnabled(state == Qt::Checked);
        ui->baseAltInput->setEnabled(state == Qt::Checked);
        ui->basePositionButton->setEnabled(state == Qt::Checked);
        ui->basePositionButton->setEnabled(state == Qt::Checked);
        if(ui->checkIBGE->isChecked()) {
            ui->checkIBGE->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        }
    });
    connect(ui->checkIBGE, &QCheckBox::stateChanged, [=](int state) {
        ui->IBGEObsInput->setEnabled(state == Qt::Checked);
        ui->IBGENavInput->setEnabled(state == Qt::Checked);
        ui->IBGENavButton->setEnabled(state == Qt::Checked);
        ui->IBGEObsButton->setEnabled(state == Qt::Checked);
        ui->label_3->setEnabled(state == Qt::Checked);
        ui->label_4->setEnabled(state == Qt::Checked);
        ui->checkIBGEConfig->setEnabled(state == Qt::Checked);
        if(ui->checkIBGEConfig->isChecked()) {
            ui->checkIBGEConfig->setCheckState(Qt::Unchecked);
        }
        if(ui->checkBaseRtk->isChecked()) {
            ui->checkBaseRtk->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        }
    });
    connect(ui->checkIBGEConfig, &QCheckBox::stateChanged, [=](int state) {
        ui->configIBGEInput->setEnabled(state == Qt::Checked);
        ui->IBGEConfigButton->setEnabled(state == Qt::Checked);
    });
    //TODO
    // We don't have it done yet
    ui->checkBaseRtk->setCheckState(Qt::Unchecked);
    ui->checkIBGE->setCheckState(Qt::Unchecked);
    ui->checkBaseRtk->setEnabled(true);
    ui->checkIBGE->setEnabled(true);

    // Connect buttons
    // Hover button
    connect(ui->hoverRawButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .UBX file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->hoverRawInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // Base button
    connect(ui->baseRawButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .UBX file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->baseRawInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // PPK
    connect(ui->configButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .conf file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.conf"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->configInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // IBGE Obs
    connect(ui->IBGEObsButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17o/.o file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.17o *.o"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGEObsInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // IBGE Nav
    connect(ui->IBGENavButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17n/.n file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.17n *.n"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGENavInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // IBGE PPP
    connect(ui->IBGEConfigButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .conf file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.conf"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->configIBGEInput->setText(QDir::toNativeSeparators(fileName));
            _findFolder = QDir(fileName).absolutePath();
        }
    });

    // Base RTK csv values
    connect(ui->basePositionButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .csv file"), \
                _findFolder.isEmpty() ? QDir::homePath() : QDir(_findFolder).absolutePath(), \
                QStringLiteral("*.csv"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            _findFolder = QDir(fileName).absolutePath();
            //TODO
            qDebug() << "TODO";
        }
    });

    // Process button
    connect(ui->processButton, &QPushButton::clicked, [=](){
        if(ui->hoverRawInput->text().isEmpty() && ui->baseRawInput->text().isEmpty()) {
            QMessageBox::critical(this, tr("Error"), tr("Add Rover and Base files !"));
            return;
        }

        if(ui->checkIBGE->checkState() == Qt::Checked) {
            if(ui->IBGEObsInput->text().isEmpty() || ui->IBGENavInput->text().isEmpty()) {
                QMessageBox::critical(this, tr("Error"), tr("Add IBGE files !"));
                return;
            }
        } else {
            if(ui->checkBaseRtk->checkState() == Qt::Checked) {
                if (ui->baseLatInput->text().isEmpty() \
                    || ui->baseLonInput->text().isEmpty() \
                    || ui->baseLonInput->text().isEmpty()) {
                    QMessageBox::critical(this, tr("Error"), tr("Add base position !"));
                    return;
                }
            }
        }

        runRTKLIB();
    });
}

int Window::runCmd(QString cmd)
{
    qDebug() << "Running";
    QProcess *process = new QProcess();
    process->setStandardOutputFile(QProcess::nullDevice());
    process->setStandardErrorFile(QProcess::nullDevice());
    ui->output->append(QStringLiteral("Running %1").arg(cmd));
    ui->output->append("");
    process->start(cmd);
    while(!process->waitForFinished(100)) {
        QCoreApplication::processEvents();
    }
    ui->output->moveCursor(QTextCursor::End);
    QCoreApplication::processEvents();
    return process->exitCode();
}

void Window::runRTKLIB()
{
    QString runningString = QStringLiteral("Running ... \t|\t%1 / %2");
    uint step = 1;
    uint nSteps = 4 + ui->checkIBGE->isChecked()*2;
    //TODO
    // Use QDir::toNativeSeparators in all paths
    ui->statusbar->showMessage("Running ...");
    QString path = qApp->applicationDirPath();
    _folderName = QDir::toNativeSeparators(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmsszzz")));
    _savedPath = path + QStringLiteral("/CUI-") + _folderName + "/";
    _savedPath = QDir::toNativeSeparators(_savedPath);
    path = QDir::toNativeSeparators(path + "/");
    QString cmd;

    // Rover convbin
    Convbin convbin;
    convbin.setBinPath(path);
    convbin.setLogFile(ui->hoverRawInput->text());
    convbin.setLogFormatType("ubx");
    convbin.setOutputFile("rover");
    convbin.setOutputPath(_savedPath);
    convbin.setRinexVersion("3.03");
    cmd = convbin.command();

    ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }

    // Base convbin
    convbin.setBinPath(path);
    convbin.setLogFile(ui->baseRawInput->text());
    convbin.setLogFormatType("ubx");
    convbin.setOutputFile("base");
    convbin.setDelta(0.0, 0.0, QString(ui->AntAltInput->text()).toFloat());
    convbin.setOutputPath(_savedPath);
    convbin.setRinexVersion("3.03");
    cmd = convbin.command();

    ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }

    if(ui->checkIBGE->isChecked()) {
        RnxToRtkp rnx2rtkp;
        rnx2rtkp.setBinPath(path);
        QString configFile = ui->checkIBGEConfig->isChecked() ? \
            ui->configIBGEInput->text() : path + "configs2.conf";
        rnx2rtkp.setConfigurationFile(configFile);
        rnx2rtkp.setInputFile(_savedPath + "base.obs");
        rnx2rtkp.setNavFile(ui->IBGENavInput->text());
        rnx2rtkp.setObsFile(ui->IBGEObsInput->text());
        rnx2rtkp.setOutputFile("ibge");
        rnx2rtkp.setOutputPath(_savedPath);
        cmd = rnx2rtkp.command();

        ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
        if(runCmd(cmd) != 0) {
            ui->output->append("Error !");
            ui->statusbar->showMessage("ERRO !");
            return;
        }

        ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
        QFile* file = new QFile(_savedPath + "ibge.pos");
        file->open(QIODevice::ReadOnly | QIODevice::Text);
        QString line;
        unsigned int lineCount = 0;
        while(!file->atEnd() && lineCount < 24){
            line = file->readLine();
            lineCount++;
        }
        QStringList lineSplited = line.split("  ");
        ui->baseLatInput->setText(lineSplited[1]);
        ui->baseLonInput->setText(lineSplited[2]);
        ui->baseAltInput->setText(lineSplited[3]);
    }

    RnxToRtkp rnx2rtkp;
    rnx2rtkp.setBinPath(path);
    QString configFile = ui->checkConfig->isChecked() ? \
            ui->configInput->text() : path + "configs.conf";
    rnx2rtkp.setConfigurationFile(configFile);
    rnx2rtkp.setElevationMask(ui->ElevationMaskInput->value());
    rnx2rtkp.setInputFile(_savedPath + "rover.obs");
    rnx2rtkp.setNavFile(_savedPath + "base.nav");
    rnx2rtkp.setObsFile(_savedPath + "base.obs");
    rnx2rtkp.setOutputFile("out");
    rnx2rtkp.setOutputPath(_savedPath);
    if(ui->checkBaseRtk->isChecked() || ui->checkIBGE->isChecked()) {
        rnx2rtkp.setBasePosition(ui->baseLatInput->text(), ui->baseLonInput->text(), ui->baseAltInput->text());
    }
    cmd = rnx2rtkp.command();

    ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }

    PosToKml pos2kml;
    pos2kml.setBinPath(path);
    pos2kml.setOutputFile("out");
    pos2kml.setOutputPath(_savedPath);
    cmd = pos2kml.command();

    ui->statusbar->showMessage(runningString.arg(step++).arg(nSteps));
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }
    ui->output->append("DONE !");
    ui->statusbar->showMessage("DONE !");

    QString pathToSave = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Select path"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    if(pathToSave.isEmpty()) {
        ui->statusbar->showMessage("ERRO !");
        return;
    }

    ui->statusbar->showMessage("Saving ...");
    if(!QDir(pathToSave + "/" + _folderName).exists())
        QDir().mkdir(pathToSave + "/" + _folderName);
    qDebug() <<  _savedPath << QDir(_savedPath).exists() <<  pathToSave + "/" + _folderName << QDir(pathToSave + "/" + _folderName).exists();
    QStringList files = QDir(_savedPath).entryList(QDir::Files);
    for(const auto file : files) {
        QFile(QDir::toNativeSeparators(_savedPath + "/" + file)).copy(QDir::toNativeSeparators(pathToSave + "/" + _folderName + "/" + QFile(file).fileName()));
    }
    ui->statusbar->showMessage("Done ! (Saved in " + QDir::toNativeSeparators(pathToSave + "/" + _folderName + "/") + ")");
}

Window::~Window()
{
    delete ui;
}
