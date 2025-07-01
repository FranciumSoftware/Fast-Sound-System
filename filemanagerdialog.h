#ifndef FILEMANAGERDIALOG_H
#define FILEMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

class FileManagerDialog : public QDialog {
    Q_OBJECT

public:
    explicit FileManagerDialog(QWidget *parent = nullptr);
    ~FileManagerDialog();

private slots:
    void onImportButtonClicked();
    void onDeleteButtonClicked();
    void onRenameButtonClicked();

private:
    QListWidget *fileListWidget;
    QPushButton *importButton;
    QPushButton *deleteButton;
    QPushButton *renameButton;

    const QString musicDir = "Musics/";
    void loadFiles();
};

#endif // FILEMANAGERDIALOG_H
