#include "window.h"
#include "ui_window.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QStandardPaths>
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
    ui->AntAltInput->setValidator(new QDoubleValidator(0, 200, 6, this));

    // Make connections and set initial state
    // IBGE and Base RTK
    connect(ui->checkBaseRtk, &QCheckBox::stateChanged, [=](int state) {
        ui->baseLatInput->setEnabled(state == Qt::Checked);
        ui->baseLonInput->setEnabled(state == Qt::Checked);
        ui->baseAltInput->setEnabled(state == Qt::Checked);
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
        if(ui->checkBaseRtk->isChecked()) {
            ui->checkBaseRtk->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        }
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
                QDir::homePath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->hoverRawInput->setText(QDir::toNativeSeparators(fileName));
        }
    });

    // Base button
    connect(ui->baseRawButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .UBX file"), \
                QDir::homePath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->baseRawInput->setText(QDir::toNativeSeparators(fileName));
        }
    });

    // IBGE Obs
    connect(ui->IBGEObsButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17o file"), \
                QDir::homePath(), \
                QStringLiteral("*.17o"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGEObsInput->setText(QDir::toNativeSeparators(fileName));
        }
    });

    // IBGE Nav
    connect(ui->IBGENavButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17n file"), \
                QDir::homePath(), \
                QStringLiteral("*.17n"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGENavInput->setText(QDir::toNativeSeparators(fileName));
        }
    });

    // Base RTK csv values
    connect(ui->basePositionButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .csv file"), \
                QDir::homePath(), \
                QStringLiteral("*.csv"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
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
    ui->output->append(QStringLiteral("Running %1").arg(cmd));
    ui->output->append("");
    process->start(cmd);
    while(!process->waitForFinished(100)) {
        QCoreApplication::processEvents();
        process->readAllStandardError();
    }
    ui->output->moveCursor(QTextCursor::End);
    QCoreApplication::processEvents();
    return process->exitCode();
}

void Window::runRTKLIB()
{
    //TODO
    // Use QDir::toNativeSeparators in all paths
    ui->statusbar->showMessage("Running ...");
    QString path = qApp->applicationDirPath();
    _folderName = QDir::toNativeSeparators(QDateTime::currentDateTime().toString(QStringLiteral("ddMMyyyy-hhmmsszzz")));
    _savedPath = path + QStringLiteral("/CUI-") + _folderName + "/";
    path = QDir::toNativeSeparators(path + "/");
    QString cmd = QStringLiteral("\"%1convbin\" -f 1 -r ubx -v 3.03 \"%2\" -d \"%3\" -o \"rover.obs\" -n \"rover.nav\"").arg(path).arg(ui->hoverRawInput->text()).arg(_savedPath);
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }
    cmd = QStringLiteral("\"%1convbin\" -hd %4/0/0 -f 1 -r ubx -v 3.03 \"%2\" -d \"%3\" -o \"base.obs\" -n \"base.nav\"").arg(path).arg(ui->baseRawInput->text()).arg(_savedPath).arg(ui->AntAltInput->text());
    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }
    _savedPath = QDir::toNativeSeparators(_savedPath);

    if(ui->checkIBGE->isChecked()) {
        cmd = QStringLiteral("\"%1rnx2rtkp\" -k \"%1configs2.conf\" \"%2base.obs\" \"%3\" \"%4\" -o \"%2ibge.pos\"").arg(path).arg(_savedPath).arg(ui->IBGEObsInput->text(), ui->IBGENavInput->text());

        if(runCmd(cmd) != 0) {
            ui->output->append("Error !");
            ui->statusbar->showMessage("ERRO !");
            return;
        } else {
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
    }

    if(ui->checkBaseRtk->isChecked() || ui->checkIBGE->isChecked()) {
        cmd = QStringLiteral("\"%1rnx2rtkp\" -k \"%1configs.conf\" -l %3 %4 %5 \"%2rover.obs\" \"%2base.obs\" \"%2base.nav\" \"%2base.gnav\" -o \"%2out.pos\"").arg(path).arg(_savedPath).arg(ui->baseLatInput->text(), ui->baseLonInput->text(), ui->baseAltInput->text());
    } else {
        cmd = QStringLiteral("\"%1rnx2rtkp\" -k \"%1configs.conf\" \"%2rover.obs\" \"%2base.obs\" \"%2base.nav\" \"%2base.gnav\" -o \"%2out.pos\"").arg(path).arg(_savedPath);
    }

    if(runCmd(cmd) != 0) {
        ui->output->append("Error !");
        ui->statusbar->showMessage("ERRO !");
        return;
    }

    cmd = QStringLiteral("\"%1pos2kml\" -o \"%2out_events.gpx\" -a -gpx \"%2out_events.pos\"").arg(path).arg(_savedPath);
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
