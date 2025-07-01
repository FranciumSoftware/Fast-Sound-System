#include "FileManagerDialog.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>
#include <QMessageBox>

FileManagerDialog::FileManagerDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Gestion des fichiers");
    setMinimumSize(400, 300);
 // Assurez-vous que ce chemin est correct dans votre environnement
    QDir().mkpath(musicDir); // Créer le répertoire s'il n'existe pas

    QVBoxLayout *layout = new QVBoxLayout(this);

    fileListWidget = new QListWidget(this);
    importButton = new QPushButton("Importer", this);
    deleteButton = new QPushButton("Supprimer", this);
    renameButton = new QPushButton("Renommer", this);

    layout->addWidget(fileListWidget);
    layout->addWidget(importButton);
    layout->addWidget(deleteButton);
    layout->addWidget(renameButton);

    connect(importButton, &QPushButton::clicked, this, &FileManagerDialog::onImportButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &FileManagerDialog::onDeleteButtonClicked);
    connect(renameButton, &QPushButton::clicked, this, &FileManagerDialog::onRenameButtonClicked);

    loadFiles();
}

void FileManagerDialog::loadFiles() {
    fileListWidget->clear();
    QDir directory(musicDir);
    QStringList files = directory.entryList(QDir::Files);
    for (const QString &file : files) {
        fileListWidget->addItem(file);
    }
}

void FileManagerDialog::onImportButtonClicked() {
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "Sélectionnez un ou plusieurs fichiers",
        "",
        "Fichiers audio (*.mp3 *.wav *.aac *.ogg *.flac *.m4a *.wma *.opus);;"
        "Fichiers MP3 (*.mp3);;"
        "Fichiers WAV (*.wav);;"
        "Fichiers AAC (*.aac);;"
        "Fichiers OGG (*.ogg);;"
        "Fichiers FLAC (*.flac);;"
        "Fichiers M4A (*.m4a);;"
        "Fichiers WMA (*.wma);;"
        "Fichiers OPUS (*.opus)"
        );
    if (!filePaths.isEmpty()) {
        for (const QString &filePath : filePaths) {
            QFile file(filePath);
            QString destPath = musicDir + QFileInfo(filePath).fileName();
            if (QFile::copy(filePath, destPath)) {
                fileListWidget->addItem(QFileInfo(filePath).fileName());
            }
        }
    }
    loadFiles();
}

void FileManagerDialog::onDeleteButtonClicked() {
    QListWidgetItem *item = fileListWidget->currentItem();
    if (item) {
        QString fileName = item->text();
        QFile file(musicDir + fileName);
        if (file.remove()) {
            delete item; // Supprimer l'élément de la liste
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de supprimer le fichier");
        }
    }
}

void FileManagerDialog::onRenameButtonClicked() {
    QListWidgetItem *item = fileListWidget->currentItem();
    if (item) {
        QString oldName = item->text();
        bool ok;
        QString newName = QInputDialog::getText(this, "Renommer le fichier", "Entrez le nouveau nom:", QLineEdit::Normal, oldName, &ok);
        if (ok && !newName.isEmpty()) {
            QFile file(musicDir + oldName);
            if (file.rename(musicDir + newName)) {
                item->setText(newName);
            } else {
                QMessageBox::warning(this, "Erreur", "Impossible de renommer le fichier");
            }
        }
    }
}

FileManagerDialog::~FileManagerDialog() {}
