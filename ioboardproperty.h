#ifndef IOBOARDPROPERTY_H
#define IOBOARDPROPERTY_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>

#include "ioboard.h"



QT_BEGIN_NAMESPACE
namespace Ui { class ioBoardPropertyDialog; }
QT_END_NAMESPACE

// IO board popup menu
class IOBoardProperty : public QDialog
{
    Q_OBJECT
public:
    IOBoardProperty(IOBoard *ioBoard);
    ~IOBoardProperty();
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
    IOBoard *board;
    std::vector <IOBoardItem *>boardlist;

};

#endif // IOBOARDPROPERTY_H
