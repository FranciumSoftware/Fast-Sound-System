#ifndef EDIT_H
#define EDIT_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QString>

namespace Ui {
class Edit;
}

class Edit : public QWidget
{
    Q_OBJECT

public:
    explicit Edit(const QString &filePath, QWidget *parent = nullptr);
    ~Edit();
signals:
    void dataSaved(); // Nouveau signal
private slots:
    void saveClicked();
    void saveExitClicked();
    void cancelClicked();
    void addRowButtonClicked(); // Nouveau slot pour le bouton Ajouter



    void on_deleteLine_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

private:
    Ui::Edit *ui;
    QStandardItemModel *model;
    QString filePath;

    void loadCSV(const QString &filePath);
    void saveCSV(const QString &filePath);
};

#endif // EDIT_H
