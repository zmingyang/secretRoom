#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>

#include "mysquare.h"
#include "signalThread.h"
#include "ifconditionoptdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
        void ShowInfo( signalMsg msg );
        void colorRed();
        void colorGreen();
        void conditionEdit();
        void stepHereSlot();
        void stopSlot();

        void edgeProperty();
        void TimerUpdate();
private:

    QTimer *mainTimer;
    Ui::MainWindow *ui;

    QGraphicsScene *nodeScene;
    QGraphicsScene *ioBoardscene;
    mysquare *msquare;

    signalThread *ch;

    QMenu *itemMenu;
    QMenu *edgeMenu;
    QMenu *ioBoardMenu;

    QAction *redAction;
    QAction *greenAction;
    QAction *stepHere;
    QAction *stop;

    QAction *condictionAction;

    QAction *PropertyAction;
    //QAction *addAction;
};
#endif // MAINWINDOW_H
