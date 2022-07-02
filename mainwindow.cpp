#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edge.h"
#include "node.h"
#include "fsm.h"

#include "mqtt.h"
#include "outputBoard.h"
#include "inputBoardproperty.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nodeScene = new QGraphicsScene(this );
    ui->graphicsView->setScene(nodeScene);


    itemMenu = menuBar()->addMenu(tr("&Node"));

    stepHere = new QAction(QString::fromLocal8Bit("运行到这"), this);
    stop = new QAction(QString::fromLocal8Bit("停止"), this);
    condictionAction = new QAction(QString::fromLocal8Bit("下一步使能"), this);




    itemMenu->addAction(stepHere);
    itemMenu->addAction(stop);

    itemMenu->addAction(condictionAction);
    itemMenu->addSeparator();

    edgeMenu = menuBar()->addMenu(tr("&Edge"));
    PropertyAction = new QAction(QString::fromLocal8Bit("属性"), this);

    edgeMenu->addAction(PropertyAction);
    edgeMenu->addSeparator();




    ioBoardMenu = menuBar()->addMenu(tr("&IOBoard"));
    redAction = new QAction(tr("&red"), this);
    greenAction = new QAction(tr("&green"), this);
    ioBoardMenu->addAction(redAction);
    ioBoardMenu->addAction(greenAction);

    connect(redAction, SIGNAL(triggered()), this, SLOT(colorRed()));
    connect(greenAction, SIGNAL(triggered()), this, SLOT(colorGreen()));
    connect(stepHere, SIGNAL(triggered()), this, SLOT(stepHereSlot()));
    connect(stop, SIGNAL(triggered()), this, SLOT(stopSlot()));
    connect(condictionAction, SIGNAL(triggered()), this, SLOT(conditionEdit()));
    connect(PropertyAction, SIGNAL(triggered()), this, SLOT(edgeProperty()));

    fsm.createNode(ui->graphicsView, nodeScene, itemMenu, edgeMenu);



    ioBoardscene = new QGraphicsScene(this );
    ui->IOBoardgraphicsView->setScene(ioBoardscene);
//    for (int i = 0; i < 2; i++)
//    {

//        IOBoard *newBoard = new IOBoard(ui->IOBoardgraphicsView, QString::fromLocal8Bit("IO板-") + QString::number(i,10), ioBoardMenu, 30);
//        ioBoardscene->addItem(newBoard);
//    }
    fsm.createBoard(ui->IOBoardgraphicsView, ioBoardscene, ioBoardMenu );


//    for (int i = 0; i < fsm.Node_Do.size(); i++)
//    {
//        fsm.Node_Do[i].n->setPos(-500, i*20);
//        scene->addItem(fsm.Node_Do[i].n);
//    }



    connectMQTT();


    qRegisterMetaType<signalMsg>("signalMsg");
    ch = new signalThread();
    connect(ch, SIGNAL(Send2UI(signalMsg)), this, SLOT(ShowInfo(signalMsg)));
    ch->start();


    //    Node *node1 = new Node(ui->graphicsView, QString::fromLocal8Bit("你好"), itemMenu);
    //    Node *node2 = new Node(ui->graphicsView, "2", itemMenu);
    //    scene->addItem(node1);
        //scene->addItem(node2);
        //scene->addItem(new Edge(node1, node2));


    mainTimer = new QTimer(this);
    connect(mainTimer, SIGNAL(timeout()), this, SLOT(TimerUpdate()));
    mainTimer->start(100);
}


void MainWindow::TimerUpdate()
{
    qDebug() << "update";
    fsm.tick();
}


void MainWindow::colorRed()
{


//    foreach (QGraphicsItem *item, scene->selectedItems()) {
//        Node *n = qgraphicsitem_cast<Node *>(item);
//         n->setColor(Qt::red);

//     }
}

void MainWindow::colorGreen()
{


}
void MainWindow::stepHereSlot()
{
    foreach (QGraphicsItem *item, nodeScene->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(item);
         n->setColor(Qt::green);

     }

}

void MainWindow::stopSlot()
{
    foreach (QGraphicsItem *item, nodeScene->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(item);
         n->setColor(Qt::red);

     }

}
void MainWindow::conditionEdit()
{

    foreach (QGraphicsItem *item, nodeScene->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(item);

         IFConditionOptDialog dlg(n);
         dlg.exec();
     }

}

void MainWindow::edgeProperty()
{

//    foreach (QGraphicsItem *item, scene->selectedItems()) {



//     }
}


void MainWindow::ShowInfo(signalMsg msg)
{

 //printf("get msg %s \n", msg.msg);
    //ioBoard.updateBoardState(msg.msg.toStdString());

//    if (msg.msg =="a")
//    {
//        fsm.Node_D[2][1]->setColor(Qt::green);
//    }
//    else
//    {
//        fsm.Node_D[2][1]->setColor(Qt::red);
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

