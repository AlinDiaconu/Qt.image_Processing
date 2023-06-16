#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageproc.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QScreen>
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>
#include <QWindow>
#include <QDialog>
#include <QtDebug>
#include <QWidget>
#include <QImage>
#include <QSaveFile>

using namespace cv;
using namespace std;
Imageproc *imageproc;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(screen()->size().width(),screen()->size().height() - 80);
    QWidget *centralWidget = this;
    QList<QPushButton *> buttons = centralWidget->findChildren<QPushButton *>();
    foreach(QPushButton *button, buttons)
    {
        button->setFixedWidth(ui->winop->size().width());
    }
    QSize size = this->size();
    MoveLabel(size,ui->label_2);
    ui->Edit->move(size.height() - ui->Edit->height() - ui->Edit->size().height() , (size.width()- ui->Edit->size().width())/2 - ui->menubar->size().height());
    QPushButton *edit = MainWindow::findChild<QPushButton *>("Edit");
    connect(edit,SIGNAL(released()),this,SLOT(EditPressed()));


    ui->Contrast->setHidden(true);
    ui->Contrast->move(ui->Edit->pos());
    ui->label_2->setFrameShape(QFrame::NoFrame());
    ui->label_2->setFixedSize(474,474);
    ui->Contrast->setDisabled(true);
    ui->Edit->setDisabled(false);
    ui->NoiceR->move(ui->Contrast->pos().x() + ui->NoiceR->width() + 50, ui->Contrast->pos().y());
    ui->NoiceR->setHidden(true);
    ui->bin->move(ui->Contrast->pos().x() - ui->bin->width() - 100,ui->Contrast->pos().y());
    ui->bin->setHidden(true);
    ui->neg->move(ui->NoiceR->pos().x() + ui->neg->width() + 100,ui->NoiceR->pos().y());
    ui->neg->setHidden(true);
    ui->winop->move(ui->bin->pos().x() - ui->winop->width() - 100,ui->NoiceR->pos().y());
    ui->winop->setHidden(true);
    ui->extraction->move(ui->Edit->pos().x() ,ui->Edit->pos().y() - 100);
    ui->extraction->setHidden(true);
    ui->gamma->move(ui->NoiceR->pos().x() ,ui->NoiceR->pos().y() - 100);
    ui->gamma->setHidden(true);
    ui->compress->move(ui->bin->pos().x() ,ui->bin->pos().y() - 100);
    ui->compress->setHidden(true);
    ui->shine->move(ui->neg->pos().x() ,ui->neg->pos().y() - 100);
    ui->shine->setHidden(true);
    ui->gray->move(ui->winop->pos().x() ,ui->winop->pos().y() - 100);
    ui->gray->setHidden(true);
    ui->hist->move(ui->extraction->pos().x() ,ui->extraction->pos().y() + 50);
    ui->hist->setHidden(true);
    ui->egalhist->move(ui->gray->pos().x() ,ui->gray->pos().y() + 50);
    ui->egalhist->setHidden(true);
    ui->convolutie->move(ui->shine->pos().x() ,ui->shine->pos().y() + 50);
    ui->convolutie->setHidden(true);


    connect(ui->Contrast,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->NoiceR,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->bin,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->neg,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->winop,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->extraction,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->compress,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->gamma,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->shine,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->gray,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->hist,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->egalhist,SIGNAL(released()),this,SLOT(NewWindow()));
    connect(ui->convolutie,SIGNAL(released()),this,SLOT(NewWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open file: ");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Cannot open file: " + file.errorString());
    }
    else{
        QPixmap pixmap(fileName);
        if(pixmap.width() > ui->label_2->width() || pixmap.height() > ui->label_2->height()){
            pixmap = pixmap.scaled(ui->label_2->size(),Qt::KeepAspectRatio);
        }
        ui->label_2->setPixmap(pixmap);
        //ui->label_2->setFixedSize(pixmap.size());
        ui->label_2->setFrameShape(QFrame::StyledPanel);
        ui->label_2->setAlignment(Qt::AlignCenter);
        MoveLabel(MainWindow::size(), ui->label_2);
    }
}

void MainWindow::EditPressed()
{
    if(!ui->label_2->pixmap().isNull())
    {
        ui->Edit->setHidden(true);
        ui->Contrast->setHidden(false);
        ui->Contrast->setDisabled(false);
        ui->Edit->setDisabled(true);
        ui->NoiceR->setHidden(false);
        ui->bin->setHidden(false);
        ui->neg->setHidden(false);
        ui->winop->setHidden(false);
        ui->extraction->setHidden(false);
        ui->compress->setHidden(false);
        ui->gamma->setHidden(false);
        ui->shine->setHidden(false);
        ui->gray->setHidden(false);
        ui->hist->setHidden(false);
        ui->egalhist->setHidden(false);
        ui->convolutie->setHidden(false);
    }
    else QMessageBox::warning(this,"Warning","You haven't opened any file!");
}

void MainWindow::MoveLabel(QSize size,QLabel *label){
    label->move((size.width() - label->width())/2,(size.height()/2 - label->height())/2 +  3 * ui->menubar->height());
}

void MainWindow::NewWindow(){
    QPushButton *buton = qobject_cast<QPushButton*>(sender());
    imageproc = new Imageproc(ui->label_2,buton,this);
    MainWindow::setDisabled(true);
    imageproc->setDisabled(false);
    imageproc->show();
    connect(imageproc,&QWidget::destroyed,this,&QWidget::setEnabled);
    connect(imageproc,&Imageproc::applyClicked,this,&MainWindow::applyClicked);
}

void MainWindow::applyClicked(){
    QPixmap pixmap = imageproc->findChild<QLabel *>("img")->pixmap();
    ui->label_2->setPixmap(pixmap);
}




void MainWindow::on_actionSave_file_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Image", "", "Image Files (*.png *.jpg *.bmp)");
    if (fileName.isEmpty()) {
        return;
    }

    if (!ui->label_2->pixmap().save(fileName)){
        QMessageBox::warning(this,"Warning","Cannot save file");
    }
}

