#ifndef IFCONDITIONOPTDIALOG_H
#define IFCONDITIONOPTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>

#include "node.h"

namespace Ui {
class IFConditionOptDialog;
}

class IFConditionOptDialog : public QDialog
{
    Q_OBJECT

public:
    class IFCondItem
    {
    public:
      QListWidgetItem *item;
      Edge * edge;
    };

    //explicit IFConditionOptDialog(QWidget *parent = nullptr);
    IFConditionOptDialog(Node *n);

    ~IFConditionOptDialog();


private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::IFConditionOptDialog *ui;
    Node *node;
    QVector<IFCondItem> itemList;

};

#endif // IFCONDITIONOPTDIALOG_H
