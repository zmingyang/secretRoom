#include "outputBoardproperty.h"
#include "ui_ioBoardProperty.h"
#include <mosquitto.h>
#include "cJSON.h"
#include "msgQueue.hpp"


outputBoardProperty::outputBoardProperty(outputIOBoard *ioBoard):
     ui(new Ui::ioBoardPropertyDialog), board(ioBoard)
{
    ui->setupUi(this);

    //DB_SCM* dbScm = fsm.search_DB_SCM_by_ID( board->node.id );

    for (auto& x: board->node->pins)
    //for(int i = 0; i < board->node->pins.size(); i++)
    {
        IOBoardItem* newItem = new (IOBoardItem);
         newItem->item = new QListWidgetItem();
         newItem->PinVal = x.second;



         //item->setText( QString::number(fsm.Node_IF[i]->IF.To_Task_D_ID,10));
         newItem->item->setText(QString::fromStdString(x.first));
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

outputBoardProperty::~outputBoardProperty()
{

    delete ui;

}


void outputBoardProperty::on_OKPushButton_clicked()
{
    for(int i = 0; i < boardlist.size(); i++)
    {
        std::string key = boardlist[i]->item->text().toStdString();
        if(boardlist[i]->item->checkState()  == Qt::Checked)
        {

            board->node->pins[key] = 0;
        }
        else
        {
            board->node->pins[key] = 1;
        }
    }

//    if(board->type == 0)//ouput board send mqtt msg
//    {
////        fsm.update_DB_SCM_IOBoard( ((DB_SCM*)(board->fsmIOBoard))->SCM_addr , board->node.Pin);
////        fsm.sentIOBoardMsg_by_BoardID( ((DB_SCM*)(board->fsmIOBoard))->SCM_addr );
//    }


    board->update();
}


void outputBoardProperty::on_CancelPushButton_clicked()
{
    close();
}

