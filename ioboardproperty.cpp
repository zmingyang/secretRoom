#include "ioboardproperty.h"
#include "ui_ioBoardProperty.h"
#include <mosquitto.h>
#include "cJSON.h"
#include "msgQueue.hpp"


IOBoardProperty::IOBoardProperty(IOBoard *ioBoard):
     ui(new Ui::ioBoardPropertyDialog), board(ioBoard)
{
    ui->setupUi(this);

    //DB_SCM* dbScm = fsm.search_DB_SCM_by_ID( board->node.id );
    for(int i = 0; i < board->node.Pin.size(); i++)
    {
        IOBoardItem* newItem = new (IOBoardItem);
         newItem->item = new QListWidgetItem();
         newItem->PinVal =board->node.Pin[i];



         //item->setText( QString::number(fsm.Node_IF[i]->IF.To_Task_D_ID,10));
         newItem->item->setText( QString::fromLocal8Bit("Pin") + QString::number(i));
         if( newItem->PinVal == 0 )
         {
            newItem->item->setCheckState(Qt::Unchecked);
         }
         else
         {
            newItem->item->setCheckState(Qt::Checked);
         }

         boardlist.push_back(newItem);

         ui->listWidget->addItem( newItem->item );


    }
}

IOBoardProperty::~IOBoardProperty()
{

    delete ui;

}


void IOBoardProperty::on_OKPushButton_clicked()
{
    for(int i = 0; i < boardlist.size(); i++)
    {
        if(boardlist[i]->item->checkState()  == Qt::Checked)
        {
            board->node.Pin[i] = 0;
        }
        else
        {
            board->node.Pin[i] = 1;
        }
    }

    if(board->type == 0)//ouput board send mqtt msg
    {
        fsm.update_DB_SCM_IOBoard( ((DB_SCM*)(board->fsmIOBoard))->id , board->node.Pin);
        fsm.sentIOBoardMsg_by_BoardID( ((DB_SCM*)(board->fsmIOBoard))->id );
    }


    board->update();
}


void IOBoardProperty::on_CancelPushButton_clicked()
{
    close();
}

