#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "filemanagerdialog.h"
#include "aubout.h"
#include <QDir>
#include <QFile>
#include "edit.h"
#include <QTextStream>
#include <QDebug>
#include <QUrl>
#include <QIcon>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player.setAudioOutput(&audioOutput);

    loadAudioList();

    connect(&player, &QMediaPlayer::errorOccurred, this, [this]() {
        qDebug() << "Media error:" << player.errorString();
        ui->label->setText("Erreur de lecture");
        QMessageBox::warning(this, "Erreur", player.errorString());
    });

    // Remplace la connexion durationChanged :
    connect(&player, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->horizontalSlider->setRange(0, duration);
        QTime totalTime = QTime::fromMSecsSinceStartOfDay(duration);
        ui->label_3->setText(totalTime.toString("mm:ss"));
    });

    // Modifie la connexion positionChanged :
    connect(&player, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        if (!ui->horizontalSlider->isSliderDown())
            ui->horizontalSlider->setValue(position);
        QTime currentTime = QTime::fromMSecsSinceStartOfDay(position);
        ui->label_2->setText(currentTime.toString("mm:ss"));
    });

    connect(ui->horizontalSlider, &QSlider::sliderMoved, &player, &QMediaPlayer::setPosition);

    connect(ui->Gslider, &QSlider::valueChanged, this, [this](int value) {
        // Volume global (0-100)
        audioOutput.setVolume(value / 100.0);
    });

    connect(ui->LSlider, &QSlider::valueChanged, this, [this](int value) {
        // Simule la balance gauche (0-100)
        // Ici, on ajuste le volume global selon la balance
        float left = value / 100.0f;
        float right = ui->RSlider->value() / 100.0f;
        float global = ui->Gslider->value() / 100.0f;
        // Moyenne pondérée pour simuler la balance
        audioOutput.setVolume(global * (left + right) / 2.0f);
    });

    connect(ui->RSlider, &QSlider::valueChanged, this, [this](int value) {
        // Simule la balance droite (0-100)
        float left = ui->LSlider->value() / 100.0f;
        float right = value / 100.0f;
        float global = ui->Gslider->value() / 100.0f;
        audioOutput.setVolume(global * (left + right) / 2.0f);
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Modify on_listWidget_currentRowChanged
void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if (currentRow >= 0 && currentRow < m_audioFiles.size()) {
        QUrl fileUrl = QUrl::fromLocalFile(m_audioFiles[currentRow]);
        player.setSource(QUrl(fileUrl));
        player.play();
        ui->PlayButton->setIcon(QIcon(":/assets/pause64px.svg"));
        ui->label->setText("Lecture en cours: " + ui->listWidget->currentItem()->text());
    }
}


void MainWindow::on_PlayButton_clicked()
{
    if (ui->listWidget->currentRow() >= 0) {
        if (player.playbackState() == QMediaPlayer::PlayingState) {
            player.pause();
            ui->PlayButton->setIcon(QIcon(":/assets/play32px.svg"));
        } else if (player.mediaStatus() == QMediaPlayer::LoadedMedia ||
                   player.mediaStatus() == QMediaPlayer::BufferedMedia) {
            player.play();
            ui->PlayButton->setIcon(QIcon(":/assets/pause64px.svg"));
            ui->label->setText("Lecture en cours: " + ui->listWidget->currentItem()->text());
        }
    }
}

void MainWindow::on_StopButton_clicked()
{
    player.stop();
    ui->label->setText("Pas de média sélectionné");
    ui->PlayButton->setIcon(QIcon(":/assets/pause64px.svg"));
    ui->horizontalSlider->setValue(0);
}

void MainWindow::on_PrevButton_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow > 0) {
        ui->listWidget->setCurrentRow(currentRow - 1);
    }
}

void MainWindow::on_NextButton_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    if (currentRow < ui->listWidget->count() - 1) {
        ui->listWidget->setCurrentRow(currentRow + 1);
    }
}


void MainWindow::on_actionQuitter_triggered()
{
    QApplication::quit();
}
void MainWindow::on_actionModifier_la_playlist_triggered()
{
    Edit *editWindow = new Edit("data.csv");
    connect(editWindow, &Edit::dataSaved, this, &MainWindow::reloadData); // Connecter le signal au slot
    editWindow->show();
}

void MainWindow::loadAudioList()
{
    ui->listWidget->clear();
    m_audioFiles.clear();

    QString filename = "data.csv";
    QFile file(filename);

    // Si le fichier n'existe pas, on le crée vide
    if (!file.exists()) {
        qDebug() << "Fichier inexistant" << Qt::endl;
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Erreur critique", "Impossible de créer le fichier data.csv.");
            return;
        }
        QTextStream out(&file);
        out << ",";
        file.close();
        QMessageBox::information(this, "Fichier inexistant", "Le fichier de playlist était inexistant, nous l'avons créé pour vous.");
        qDebug() << "Fichier créé" << Qt::endl;
    }

    // Ouvre le fichier en lecture
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier data.csv en lecture.");
        return;
    }

    QDir folder("./Musics/");
    if (!folder.exists()) {
        folder.mkdir(".");
        QMessageBox::information(this, "Dossier inexistant", "Le dossier Musics était inexistant, nous l'avons créé pour vous.");
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(',');
        if (values.size() >= 2 && !values[0].trimmed().isEmpty() && !values[1].trimmed().isEmpty()) {
            ui->listWidget->addItem(values[0].trimmed());
            QString audioFilePath = values[1].remove('"').trimmed();
            m_audioFiles.append("Musics/" + audioFilePath);
        }
    }

    file.close();
}

void MainWindow::reloadData()
{
    loadAudioList();
}


void MainWindow::on_actionG_rer_les_fichiers_triggered()
{
    FileManagerDialog dialog(this);
    dialog.exec();
}


void MainWindow::on_action_propos_triggered()
{
    aubout show(this);
    show.exec();
}


void MainWindow::on_pushButton_clicked()
{
    Edit *editWindow = new Edit("data.csv");
    connect(editWindow, &Edit::dataSaved, this, &MainWindow::reloadData); // Connecter le signal au slot
    editWindow->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    FileManagerDialog dialog(this);
    dialog.exec();
}


void MainWindow::on_pushButton_3_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_4_clicked()
{
    aubout show(this);
    show.exec();
}


