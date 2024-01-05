#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

void check_show(int value);
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int w =ui->X_Palm->width();
    int h =ui->X_Palm->height();
    int Position,V_T=0,V_I=0,V_M=0,V_R=0,V_P=0;

    //khai bao pixmap
    QPixmap None("D:/The beggin/University/DA/hand/X/PNG/None");
    //palm & thumb
    QPixmap pix_Palm_0("D:/The beggin/University/DA/hand/X/PNG/X_Palm_T_0");
    QPixmap pix_Palm_1("D:/The beggin/University/DA/hand/X/PNG/X_Palm_T_1");
    QPixmap pix_Palm_2("D:/The beggin/University/DA/hand/X/PNG/X_Palm_T_2");
    //Index
    QPixmap pix_I_1("D:/The beggin/University/DA/hand/X/PNG/X_I_1");
    QPixmap pix_I_2("D:/The beggin/University/DA/hand/X/PNG/X_I_2");
    //Middle
    QPixmap pix_M_1("D:/The beggin/University/DA/hand/X/PNG/X_M_1");
    QPixmap pix_M_2("D:/The beggin/University/DA/hand/X/PNG/X_M_2");
    //Ring
    QPixmap pix_R_1("D:/The beggin/University/DA/hand/X/PNG/X_R_1");
    QPixmap pix_R_2("D:/The beggin/University/DA/hand/X/PNG/X_R_2");
    //pinky
    QPixmap pix_P_1("D:/The beggin/University/DA/hand/X/PNG/X_P_1");
    QPixmap pix_P_2("D:/The beggin/University/DA/hand/X/PNG/X_P_2");

    ui->X_I->setPixmap(pix_I_1.scaled(w,h,Qt::KeepAspectRatio));
    ui->X_M->setPixmap(pix_M_1.scaled(w,h,Qt::KeepAspectRatio));
    ui->X_R->setPixmap(pix_R_1.scaled(w,h,Qt::KeepAspectRatio));
    ui->X_P->setPixmap(pix_P_1.scaled(w,h,Qt::KeepAspectRatio));
    ui->X_Palm->setPixmap(pix_Palm_1.scaled(w,h,Qt::KeepAspectRatio));

//    QPixmap pix_P("D:/The beggin/University/DA/hand/X/PNG/X_P_2");
//    ui->X_P->setPixmap(pix_P.scaled(w,h,Qt::KeepAspectRatio));

    int test=12;
    if(test==1){
        ui->Status_label->setText("Connected!");
        ui->Status_label->setStyleSheet("QLabel { color : green; }");
    }
    else{
        ui->Status_label->setText("Disconnected!");
        ui->Status_label->setStyleSheet("QLabel { color : red; }");
    }
    ui->X_I->setPixmap(pix_I_2.scaled(w,h,Qt::KeepAspectRatio));
   ui->X_I->setPixmap(pix_I_1.scaled(w,h,Qt::KeepAspectRatio));
    ui->X_I->setVisible(false);
//    ui->X_M->setPixmap(None.scaled(w,h,Qt::KeepAspectRatio));
//    ui->X_R->setPixmap(None.scaled(w,h,Qt::KeepAspectRatio));
//    ui->X_P->setPixmap(pix_P_1.scaled(w,h,Qt::KeepAspectRatio));
//    ui->X_Palm->setPixmap(pix_Palm_0.scaled(w,h,Qt::KeepAspectRatio));

//    switch (Position);{
//    case 0: check_show(V_T);break;
//    case 1: break;
//    case 2: break;
//    case 3: break;
//    case 4: break;
//    }

//    if(I==3){
//      ui->X_I->setPixmap(pix_I_1.scaled(w,h,Qt::KeepAspectRatio));
//    }if(I==1){
//      ui->X_I->setPixmap(pix_I_2.scaled(w,h,Qt::KeepAspectRatio));
//    }else{
//      ui->X_I->setPixmap(None.scaled(w,h,Qt::KeepAspectRatio));
//    }
}

//////////////////////NOTE///////////////////////
//There two value in this format: Position-value
//Position is for which finger it will be 0-1-2-3-4
//                                        T-I-M-R-P
//value is for how bend is the finger 0   - 1    - 2    - 3
//                                    bent-upperB-lowerB-unbent

MainWindow::~MainWindow()
{
    delete ui;
}
void check_show(int value){

}
//void MainWindow::Status_label(){
//    ui->Status_label->setText("Disconnected!");
//    ui->Status_label->setStyleSheet("QLabel { color : red; }");
//    ui->Status_label->setText("Connected!");
//    ui->Status_label->setStyleSheet("QLabel { color : green; }");
//}
