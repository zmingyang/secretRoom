#ifndef INPUTBOARDPROPERTY_H
#define INPUTBOARDPROPERTY_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>

#include "inputBoard.h"



QT_BEGIN_NAMESPACE
namespace Ui { class ioBoardPropertyDialog; }
QT_END_NAMESPACE

// IO board popup menu
class inputBoardProperty : public QDialog
{
    Q_OBJECT
public:
    inputBoardProperty(inputIOBoard *ioBoard);
    ~inputBoardProperty();
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
    inputIOBoard *board;
    std::vector <IOBoardItem *>boardlist;

};

#endif // INPUTBOARDPROPERTY_H
