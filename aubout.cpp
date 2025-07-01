#include "aubout.h"
#include "ui_aubout.h"
#include <QPixmap>
#include <QSysInfo>

aubout::aubout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::aubout)
{
    ui->setupUi(this);
    QPixmap icon(":/assets/icon.png");
    ui->label->setPixmap(icon);
    ui->textBrowser->setText("Fast-Sound-System alpha 0.1\nCreated by Francium Software under MIT liscence.\n"
                             "On :"+ QSysInfo::prettyProductName()+
                             "\n CPU architecture: "+QSysInfo::buildCpuArchitecture());
}

aubout::~aubout()
{
    delete ui;
}
