#ifndef OUTPUTBOARDPROPERTY_H
#define OUTPUTBOARDPROPERTY_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>

#include "outputBoard.h"



QT_BEGIN_NAMESPACE
namespace Ui { class ioBoardPropertyDialog; }
QT_END_NAMESPACE

// IO board popup menu
class outputBoardProperty : public QDialog
{
    Q_OBJECT
public:
    outputBoardProperty(outputIOBoard *ioBoard);
    ~outputBoardProperty();
    class IOBoardItem
    {
    public:
      QListWidgetItem *item;
      int PinVal;
    };




private slots:
    //void on_OKButton_clicked();

    //void on_cancelButton_clicked();
    void on_OKPushButton_clicked();

    void on_CancelPushButton_clicked();

private:
    void generateMQTTMsg();
    Ui::ioBoardPropertyDialog *ui;
    outputIOBoard *board;
    std::vector <IOBoardItem *>boardlist;

};

#endif // INPUTBOARDPROPERTY_H
