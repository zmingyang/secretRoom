#include "ifconditionoptdialog.h"
#include "ui_ifconditionoptdialog.h"
#include <QLabel>
#include <QCheckBox>

#include "fsm.h"

//IFConditionOptDialog::IFConditionOptDialog(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::IFConditionOptDialog)
//{
//    ui->setupUi(this);
//}

IFConditionOptDialog::IFConditionOptDialog(Node *n):
    ui(new Ui::IFConditionOptDialog),node(n)
{
    ui->setupUi(this);

    int nodeID = node->node.id;
    for(int i = 0; i < fsm.Node_IF.size(); i++)
    {
        if(fsm.Node_IF[i]->IF.From_Task_D_ID == nodeID)
        {

         IFCondItem oneItem;
         oneItem.item = new QListWidgetItem();
         oneItem.edge = fsm.Node_IF[i];
         //item->setText( QString::number(fsm.Node_IF[i]->IF.To_Task_D_ID,10));
         oneItem.item->setText( QString::fromLocal8Bit(fsm.Node_IF[i]->IF.Dscr.c_str()));
         oneItem.item->setCheckState(Qt::Unchecked);
         ui->listWidget->addItem( oneItem.item );
         itemList.push_back(oneItem);
        }
    }

}


IFConditionOptDialog::~IFConditionOptDialog()
{
    delete ui;
}

void IFConditionOptDialog::on_OKButton_clicked()
{

    for(int i = 0; i< itemList.size(); i++)
    {
        if(itemList[i].item->checkState() == Qt::Checked)
        {
            Node *n = NULL;
            int ret = fsm.getNode_D_by_ID(itemList[i].edge->IF.To_Task_D_ID, &n);
            if (ret == 0)
            {
                n->setColor(Qt::green);
            }
        }
        else
        {
            Node *n = NULL;
            int ret = fsm.getNode_D_by_ID(itemList[i].edge->IF.To_Task_D_ID, &n);
            if (ret == 0)
            {
                n->setColor(Qt::red);
            }
        }
    }

}


void IFConditionOptDialog::on_cancelButton_clicked()
{
    printf("cancel");
    this->close();
}

