#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QThread>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_PlayButton_clicked();
    void on_StopButton_clicked();
    void on_PrevButton_clicked();
    void on_NextButton_clicked();

    void on_actionModifier_la_playlist_triggered();

    void on_actionQuitter_triggered();
    void loadAudioList();
    void on_actionG_rer_les_fichiers_triggered();

    void on_action_propos_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

public slots:
    void reloadData();
private:
    Ui::MainWindow *ui;
    QMediaPlayer player;
    QAudioOutput audioOutput;
    QStringList m_audioFiles;
signals:
    void loadAudio(const QString &filePath); // Déclaration du signal
};

#endif // MAINWINDOW_H
