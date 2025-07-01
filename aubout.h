#ifndef AUBOUT_H
#define AUBOUT_H

#include <QDialog>

namespace Ui {
class aubout;
}

class aubout : public QDialog
{
    Q_OBJECT

public:
    explicit aubout(QWidget *parent = nullptr);
    ~aubout();

private:
    Ui::aubout *ui;
};

#endif // AUBOUT_H
