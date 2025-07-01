#include "edit.h"
#include "ui_edit.h"
#include "comboboxdelegate.h"
#include <QFile>
#include <QTextStream>

Edit::Edit(const QString &filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Edit),
    filePath(filePath)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    // Utiliser le délégué personnalisé pour la deuxième colonne
    ComboBoxDelegate *delegate = new ComboBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(1, delegate);

    // Charger le fichier CSV
    loadCSV(filePath);

    // Connexions des boutons
    connect(ui->save, &QPushButton::clicked, this, &Edit::saveClicked);
    connect(ui->saveExit, &QPushButton::clicked, this, &Edit::saveExitClicked);
    connect(ui->Cancel, &QPushButton::clicked, this, &Edit::cancelClicked);
    connect(ui->addRowButton, &QPushButton::clicked, this, &Edit::addRowButtonClicked);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}

void Edit::addRowButtonClicked()
{
    // Ajouter une nouvelle ligne vide
    QList<QStandardItem *> newRow;
    for (int column = 0; column < model->columnCount(); ++column) {
        newRow.append(new QStandardItem(""));
    }
    model->appendRow(newRow);
}

Edit::~Edit()
{
    delete ui;
}

void Edit::loadCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList data = line.split(",");
        QList<QStandardItem *> standardItemsList;
        for (int i = 0; i < data.size(); ++i) {
            standardItemsList.append(new QStandardItem(data[i]));
        }
        model->appendRow(standardItemsList);
    }
    file.close();
}

void Edit::saveCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (int i = 0; i < model->rowCount(); ++i) {
        QStringList rowStrings;
        for (int j = 0; j < model->columnCount(); ++j) {
            rowStrings << model->data(model->index(i, j)).toString();
        }
        out << rowStrings.join(",") << "\n";
    }
    file.close();
}

void Edit::saveClicked()
{
    saveCSV(filePath);
}

void Edit::saveExitClicked()
{
    saveCSV(filePath);
    emit dataSaved(); // Émettre le signal
    this->close();
}

void Edit::cancelClicked()
{
    this->close();
}

void Edit::on_deleteLine_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid()) {
        // Supprimer la ligne sélectionnée
        model->removeRow(index.row());
    }
}


void Edit::on_upButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid() && index.row() > 0) {
        int currentRow = index.row();
        model->insertRow(currentRow - 1, model->takeRow(currentRow));
        ui->tableView->setCurrentIndex(model->index(currentRow - 1, index.column()));
    }
}


void Edit::on_downButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.isValid() && index.row() < model->rowCount() - 1) {
        int currentRow = index.row();
        model->insertRow(currentRow + 1, model->takeRow(currentRow));
        ui->tableView->setCurrentIndex(model->index(currentRow + 1, index.column()));
    }
}

