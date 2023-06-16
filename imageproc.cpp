#include "imageproc.h"
#include "ui_imageproc.h"
#include "mainwindow.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

using namespace cv;

bool verif = false;

Imageproc::Imageproc(QLabel *image, QPushButton *buton, QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Imageproc)
{
    ui->setupUi(this);
    QWidget *centralWidget = this;
    QList<QPushButton *> buttons = centralWidget->findChildren<QPushButton *>();
    foreach(QPushButton *button, buttons)
    {
        button->setFixedWidth(ui->edgeDet->size().width());
    }
    ui->img->setPixmap(image->pixmap());
    ui->img->setFixedSize(image->size());
    ui->img->move(image->pos());
    this->setFixedSize(parent->size());
    ui->aValue->setHidden(true);
    ui->bValue->setHidden(true);
    ui->apply->move(ui->img->pos().x()  + 150, this->height() - 100);
    ui->discard->move(ui->img->pos().x()+ ui->img->size().width() - 100 ,this->height() - 100);
    ui->SA->setHidden(true);
    ui->SB->setHidden(true);
    ui->lowlim->setHidden(true);
    ui->upplim->setHidden(true);
    ui->blur->setHidden(true);
    ui->sharpen->setHidden(true);
    ui->edgeDet->setHidden(true);
    ui->median->setHidden(true);
    float h[256];
    this->updatesEnabled();
    if(buton->text() == "Contrast"){
        ui->SA->setHidden(false);
        ui->SB->setHidden(false);
        ui->lowlim->setHidden(false);
        ui->upplim->setHidden(false);
        ui->SA->move((this->width() + 2 * ui->img->size().width())/2, ui->img->pos().y() + 100);
        ui->SB->move((this->width() + 2 * ui->img->size().width())/2, ui->SA->pos().y() + 100);
        ui->lowlim->move(ui->SA->pos().x() - ui->lowlim->size().width(),ui->SA->pos().y());
        ui->upplim->move(ui->SB->pos().x() - ui->upplim->size().width(),ui->SB->pos().y());
        ui->aValue->setHidden(false);
        ui->bValue->setHidden(false);
        ui->aValue->setFixedWidth(ui->img->width());
        ui->bValue->setFixedWidth(ui->img->width());
        ui->aValue->move(ui->img->pos().x(),(this->height() + ui->img->height())/2 - 80);
        ui->bValue->move(ui->img->pos().x(),ui->aValue->pos().x());
        ui->aValue->setMaximum(254);
        ui->aValue->setMinimum(1);
        ui->bValue->setMaximum(254);
        ui->bValue->setMinimum(1);
        imag = ui->img->pixmap().toImage();
        connect(ui->aValue, SIGNAL(valueChanged(int)), this, SLOT(on_aValue_valueChanged(int)));
        connect(ui->bValue, SIGNAL(valueChanged(int)), this, SLOT(on_bValue_valueChanged(int)));
        connect(ui->SA,SIGNAL(textChanged(const QString)), this, SLOT(on_SA_textChanged(const QString)));
        connect(ui->SB,SIGNAL(textChanged(const QString)), this, SLOT(on_SB_textChanged(const QString)));
    }else if(buton->text() == "Noise Reduction"){
        ui->aValue->setHidden(false);
        ui->bValue->setHidden(false);
        ui->aValue->setFixedWidth(ui->img->width());
        ui->bValue->setFixedWidth(ui->img->width());
        ui->aValue->move(ui->img->pos().x(),(this->height() + ui->img->height())/2 - 80);
        ui->bValue->move(ui->img->pos().x(),ui->aValue->pos().x());
        ui->aValue->setMaximum(254);
        ui->aValue->setMinimum(1);
        ui->bValue->setMaximum(254);
        ui->bValue->setMinimum(1);
        imag = ui->img->pixmap().toImage();
        connect(ui->aValue, SIGNAL(valueChanged(int)), this, SLOT(on_aValue_valueChanged(int)));
        connect(ui->bValue, SIGNAL(valueChanged(int)), this, SLOT(on_bValue_valueChanged(int)));
    }else if(buton->text() == "Binarization"){
        ui->aValue->setHidden(false);
        ui->aValue->setFixedWidth(ui->img->width());
        ui->aValue->move(ui->img->pos().x(),(this->height() + ui->img->height())/2 - 80);
        ui->aValue->setMaximum(254);
        ui->aValue->setMinimum(1);
        imag = ui->img->pixmap().toImage();
        connect(ui->aValue, SIGNAL(valueChanged(int)), this, SLOT(on_aValue_Bin(int)));
    }else if(buton->text() == "Negative"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = Negative(src);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
    }else if(buton->text() == "Windows Operations"){
        ui->SA->setHidden(false);
        ui->lowlim->setHidden(false);
        ui->SA->move((this->width() + 2 * ui->img->size().width())/2, ui->img->pos().y() + 100);
        ui->lowlim->setText("r: ");
        ui->lowlim->move(ui->SA->pos().x() - ui->lowlim->size().width(),ui->SA->pos().y());
        ui->aValue->setHidden(false);
        ui->bValue->setHidden(false);
        ui->aValue->setFixedWidth(ui->img->width());
        ui->bValue->setFixedWidth(ui->img->width());
        ui->aValue->move(ui->img->pos().x(),(this->height() + ui->img->height())/2 - 80);
        ui->bValue->move(ui->img->pos().x(),ui->aValue->pos().x());
        ui->aValue->setMaximum(254);
        ui->aValue->setMinimum(1);
        ui->bValue->setMaximum(254);
        ui->bValue->setMinimum(1);
        imag = ui->img->pixmap().toImage();
        connect(ui->aValue, SIGNAL(valueChanged(int)), this, SLOT(on_aValue_WinOp(int)));
        connect(ui->bValue, SIGNAL(valueChanged(int)), this, SLOT(on_bValue_WinOp(int)));
        connect(ui->SA,SIGNAL(textChanged(const QString)), this, SLOT(on_SA_WinOp(const QString)));
    }else if(buton->text() == "Compression"){
        ui->SA->setHidden(false);
        ui->SA->move((this->width() + 2 * ui->img->size().width())/2, ui->img->pos().y() + 100);
        imag = ui->img->pixmap().toImage();
        connect(ui->SA,SIGNAL(textChanged(const QString)), this, SLOT(on_SA_Compress(const QString)));
    }else if(buton->text() == "Gamma Correction"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = Gamma(src);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
    }else if(buton->text() == "Shining"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = Stralucire(src);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
    }else if(buton->text() == "To Gray"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2RGB);
        Mat processed_image = RGBtoGray(src);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_Grayscale8));
        ui->img->setPixmap(pixmap);
    }else if(buton->text() == "Histogram"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = Histogram(src,h);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
        ui->apply->setHidden(true);
        ui->discard->setText("Back");
    }else if(buton->text() == "Egalized Histogram"){
        imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = EgalizareHisto(src);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
        ui->apply->setHidden(true);
        ui->discard->setText("Back");
    }else if(buton->text() == "Convolution"){
        imag = ui->img->pixmap().toImage();
        ui->blur->move(ui->apply->pos().x(),ui->apply->pos().y()-100);
        ui->blur->setText("Blur");
        ui->blur->setHidden(false);
        ui->sharpen->move(ui->blur->pos().x() + ui->blur->size().width() + 50, ui->blur->pos().y());
        ui->sharpen->setText("Sharpen");
        ui->sharpen->setHidden(false);
        ui->edgeDet->move(ui->blur->pos().x() - ui->edgeDet->size().width() - 50, ui->blur->pos().y());
        ui->edgeDet->setText("Edge Detection");
        ui->edgeDet->setHidden(false);
        ui->edgeDet->setFixedWidth(100);
        ui->median->move(ui->sharpen->pos().x() + ui->sharpen->size().width() + 50,ui->sharpen->pos().y());
        ui->median->setHidden(false);
    }else if(buton->text() == "Extraction"){
        ui->apply->setHidden(true);
        ui->discard->setText("Back");
        ui->discard->move(this->size().width() - ui->discard->size().width() - 100, (this->size().height() + ui->discard->size().height())/2);
        QImage imag = ui->img->pixmap().toImage();
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Extraction(src);
    }
    connect(ui->apply,SIGNAL(released()),this,SLOT(on_apply_released()));

}
Imageproc::~Imageproc(){
    delete ui;
}
//###########################################Contrast + Noise Reduction################################################################################
void Imageproc::on_SA_textChanged(const QString &arg1)
{

    Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
    cvtColor(src,src,COLOR_RGBA2GRAY);
    Mat processed_image = Contrast(src, ui->aValue->value(), ui->bValue->value(), arg1.toInt(), ui->SB->text().toInt());
    cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
    ui->img->setPixmap(pixmap);
}


void Imageproc::on_SB_textChanged(const QString &arg2)
{
    Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
    cvtColor(src,src,COLOR_RGBA2GRAY);
    Mat processed_image = Contrast(src, ui->bValue->value(), ui->bValue->value(), ui->SA->text().toInt(), arg2.toInt());
    cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
    ui->img->setPixmap(pixmap);
}




void Imageproc::on_aValue_valueChanged(int value)
{
    Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
    cvtColor(src,src,COLOR_RGBA2GRAY);
    Mat processed_image = Contrast(src, value, ui->bValue->value(), ui->SA->text().toInt(), ui->SB->text().toInt());
    cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
    ui->img->setPixmap(pixmap);
}

void Imageproc::on_bValue_valueChanged(int value)
{
    Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
    cvtColor(src,src,COLOR_RGBA2GRAY);
    Mat processed_image = Contrast(src, ui->aValue->value(), value, ui->SA->text().toInt(), ui->SB->text().toInt());
    cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
    ui->img->setPixmap(pixmap);
    this->update();
}

Mat Imageproc::Contrast(Mat src, int a, int b, int sa, int sb){
        uchar pixel;
        Mat dst;
        src.copyTo(dst);
        if (a == 0) a = 1;
        if (b == 0) b = 1;
        if (a == b)a++;
        if (b == 255) b--;
        if (a == 255) a--;
        if (sa == sb) sb++;
        if (sb == 255) sb = 254;
        if (a > b) {
            int aux = a;
            a = b;
            b = aux;
        }
        if (sa > sb) {
            int aux = sa;
            sa = sb;
            sb = aux;
        }
        int m = sa / a;
        int n = (sb - sa) / (b - a);
        int p = (255 - sb) / (255 - b);
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                if (0 < pixel && pixel <= a)
                    dst.at<uchar>(i, j) =m * pixel;
                else if (pixel > a && pixel <= b)
                    dst.at<uchar>(i, j) = n * (pixel - a) + sa;
                else if (pixel > b && pixel < 255)
                    dst.at<uchar>(i, j) = p * (pixel - b) + sb;
            }
        }
        return dst;
}
//binarizare
void Imageproc::on_aValue_Bin(int value){
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = ImBin(src,value);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}

Mat Imageproc::ImBin(Mat src, int a) {
        uchar pixel;
        Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                if (0 < pixel && pixel <= a)
                    dst.at<uchar>(i, j) = 0;
                else if (pixel > a && pixel <= 255)
                    dst.at<uchar>(i, j) = 255;
            }
        }
        return dst;
}
//negative
Mat Imageproc::Negative(Mat src){
        uchar pixel;
        Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                dst.at<uchar>(i, j) = 255 - pixel;
            }
        }
        return dst;
}
//windows operations
void Imageproc::on_aValue_WinOp(int value){
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = WinOp(src, value, ui->bValue->value(), ui->SA->text().toInt());
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}
void Imageproc::on_bValue_WinOp(int value){
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = WinOp(src, ui->aValue->value(), value, ui->SA->text().toInt());
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}
void Imageproc::on_SA_WinOp(const QString &arg3){
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = WinOp(src, ui->aValue->value(), ui->bValue->value(), arg3.toInt());
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}

Mat Imageproc::WinOp(Mat src, int a, int b, int r) {
        uchar pixel;
        Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                if (a < pixel && pixel < b)
                    dst.at<uchar>(i, j) = 255;
                else
                    dst.at<uchar>(i, j) = r;

            }
        }
        return dst;
}

//Compression
void Imageproc::on_SA_Compress(const QString &arg3){
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = Compressed(src, arg3.toInt());
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}
Mat Imageproc::Compressed(Mat src, int c){
        uchar pixel;
        Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                dst.at<uchar>(i, j) = c * log(1 + pixel);
            }
        }
        return dst;
}

//Gamma Correction

Mat Imageproc::Gamma(Mat src){
        double gamma = 2.5;
        double l = 255;
        Mat dst;
        src.copyTo(dst);
        uchar pixel;
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                dst.at<uchar>(i, j) =  l * pow(pixel / l, gamma);
            }
        }
        return dst;
}

//stralucire
Mat Imageproc::Stralucire(Mat src){
        int D = 50;
        Mat dst;
        src.copyTo(dst);
        uchar pixel;
        for (int i = 0; i < dst.rows; i++)
        {
            for (int j = 0; j < dst.cols; j++)
            {
                pixel = dst.at<uchar>(i, j);
                if (pixel + D < 255)
                    dst.at<uchar>(i, j) = pixel + D;
                else { dst.at<uchar>(i, j) = 255; }
            }
        }
        return dst;
}

//Gray
Mat Imageproc::RGBtoGray(Mat src){
        uchar pixel[3];
        Mat dst;
        src.copyTo(dst);
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols * 3; j += 3)
            {
                pixel[0] = dst.at<uchar>(i, j);
                pixel[1] = dst.at<uchar>(i, j + 1);
                pixel[2] = dst.at<uchar>(i, j + 2);
                dst.at<uchar>(i, j / 3) = (pixel[0] + pixel[1] + pixel[2]) / 3;
            }
        }
        return dst;
}

//Histograma
Mat Imageproc::Histogram(Mat src, float *h){
        unsigned long int *lh = new unsigned long int[256];
        unsigned long int n;

        int H = 400;
        int W = 512;
        int bin_w = cvRound((double)W / 256);
        Mat histImage(H, W, CV_8UC1, Scalar(255, 255, 255));

        float prrk[256], pssk[256];
        for (int i = 0; i < 256; i++)
            lh[i] = 0;
        for (int i = 0; i < src.rows; i++)
            for (int j = 0; j < src.cols; j++)
                lh[(unsigned int)src.at<uchar>(i, j)]++;

        n = H * W;
        int max = lh[0];
        for (int i = 0; i < 256; i++) {
            if (max < lh[i]) {
                max = lh[i];
            }
        }

        for (int i = 0; i < 256; i++)
        {
            h[i] = lh[i];
            lh[i] = ((double)lh[i] / max) * H;
        }


        for (int i = 0; i < 256; i++) {
            line(histImage, Point(bin_w * (i), H), Point(bin_w * (i), H - lh[i]), Scalar(0, 0, 0), 1, 8, 0);
        }
        return histImage;
}

//Egalizare Histograma
Mat Imageproc::EgalizareHisto(Mat a){
        Mat b;
        float h[256];
        float g[256];
        Histogram(a, h);
        int H = 400;
        int W = 512;
        float prrk[256], pssk[256],final[256];

        for (int i = 0; i < 256; i++) {
            prrk[i] = 0;
            pssk[i] = 0;
            final[i] = 0;
            g[i] = 0;
        }

        for (int i = 0; i < 256; i++)
            prrk[i] = h[i] / (a.rows * a.cols);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j <= i; j++)
                pssk[i] += prrk[j];
        for (int i = 0; i < 256; i++)
            final[i] = cvRound(pssk[i] * 255);

        for (int i = 0; i < 256; i++)
            for (int j = 0; j < 256; j++)
                if (final[i] == final[j] && i != j)
                    final[i] += final[j];

        for (int i = 0; i < 256; i++)
            g[cvRound(pssk[i] * 255)] = cvRound(prrk[i] * (H * W));

        for(int i=0;i<256;i++)
            for(int j=0;j<256;j++)
                if (g[i] == final[j] && i != j)
                {
                    g[i] += g[j];
                }

        b = a.clone();
        for (int i = 0; i < a.rows; i++)
            for (int j = 0; j < a.cols; j++)
                b.at<uchar>(i, j) = saturate_cast<uchar>(final[a.at<uchar>(i, j)]);

        int bin_w = cvRound((double)W/ 256);

        Mat histImage(H, W, CV_8U, Scalar(255, 255, 255));


        for (int i = 0; i < 256; i++) {
            line(histImage, Point(bin_w * (i), H), Point(bin_w * (i),H - (int)g[i] % 256), Scalar(0, 0, 0), 1, 8, 0);
        }
        imshow("Noua imagine: ", b);
        return histImage;
}

//Convolutie
void Imageproc::on_blur_released()
{
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        float w[100][100] = {{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9}};
        Mat processed_image = convolutie(src,3,3, w);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}



void Imageproc::on_sharpen_released()
{
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        float w[100][100] = {{0,-1,0},{-1,5,-1},{0,-1,0}};
        Mat processed_image = convolutie(src,3,3, w);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}

void Imageproc::on_edgeDet_released()
{
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        float w[100][100] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};
        Mat processed_image = convolutie(src,3,3, w);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}

Mat Imageproc::convolutie(Mat src,int m,int n, float w[100][100]) {
        Mat dst;
        src.copyTo(dst);
        int a = (m - 1) / 2, b = (n - 1) / 2;
        src.convertTo(src, CV_32FC1);
        dst.convertTo(dst, CV_32FC1);
        for(int i=a;i<src.rows-a;i++)
            for (int j = b; j < src.cols-b; j++)
            {
                float sum = 0;
                for (int k = -a; k <= a; k++)
                {
                    for (int l = -b; l <= b; l++)
                    {
                        sum += w[a+k][a+l] * src.at<float>(i + k, j + l);
                    }
                }
                dst.at<float>(i, j) = sum;
            }
        dst.convertTo(dst, CV_8UC1);
        return dst;
}

void Imageproc::on_median_released()
{
        Mat src(imag.height(),imag.width(),CV_8UC4,imag.bits(),imag.bytesPerLine());
        cvtColor(src,src,COLOR_RGBA2GRAY);
        Mat processed_image = MedianFilter(src,3);
        cvtColor(processed_image,processed_image,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)processed_image.data,processed_image.cols,processed_image.rows,processed_image.step,QImage::Format_RGBA8888));
        ui->img->setPixmap(pixmap);
}

Mat Imageproc::MedianFilter(Mat src, int m){
        Mat dst;
        src.copyTo(dst);
        int a = (m - 1) / 2;
        for (int i = a; i < src.rows - a ; i++)
            for (int j = a; j < src.cols - a; j++)
            {
                int n = 0;
                float sum[100];
                for (int k = -a; k <= a; k++)
                {
                    for (int l = -a; l <= a; l++)
                    {
                        sum[n] = src.at<uchar>(i + k, j + l);
                        n++;
                    }
                }
                for (int k = 0; k < n-1 ; k++)
                {
                    for (int l = k+1; l < n; l++)
                    {
                        if (sum[k] > sum[l])
                        {
                            float aux;
                            aux = sum[l];
                            sum[l] = sum[k];
                            sum[k] = aux;
                        }
                    }
                }
                dst.at<uchar>(i, j) = sum[(m * m) / 2];
            }
        return dst;
}

//Extraction
void Imageproc::Extraction(Mat src){
        uchar pixel;
        Mat vm[8];
        Size outsize(237,237);
        Mat dst;
        cv::resize(src,dst,outsize,INTER_LINEAR);
        for (int i = 0; i <= 7; i++) {
            vm[i] = Mat::zeros(src.size(), src.type());
        }
        for (int k = 0; k < 8; k++)
        {
            for (int i = 0; i < src.rows; i++)
            {
                for (int j = 0; j < src.cols; j++)
                {
                    pixel = src.at<uchar>(i, j);
                    vm[k].at<uchar>(i, j) = pixel % 2 * 255;
                    src.at<uchar>(i, j) /= 2;

                }
            }
        }
        Mat merged(237 * 3, 237 * 3 ,0);
        int xpos=0,ypos=0;
        for(int i=0;i<8;i++){
            if(i == 3) {ypos = 237;xpos = 0;}
            if(i == 6) {ypos = 237 * 2;xpos = 0;}
                cv::resize(vm[i],vm[i],outsize,INTER_LINEAR);
                vm[i].copyTo(merged(Rect(xpos,ypos,vm[i].rows,vm[i].cols)));
                xpos += 237;
        }
        dst.copyTo(merged(Rect(xpos,ypos,dst.rows,dst.cols)));
        cvtColor(merged,merged,COLOR_GRAY2RGBA);
        QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)merged.data,merged.cols,merged.rows,merged.step,QImage::Format_RGBA8888));
        ui->img->setFixedSize(pixmap.size());
        ui->img->setPixmap(pixmap);
        MoveLabel(this->size(),ui->img);


}

void Imageproc::MoveLabel(QSize size,QLabel *label){
        label->move((size.width() - label->width())/2,(size.height() - label->height())/2);
}

void Imageproc::closeEvent(QCloseEvent *event){
        event->accept();
        parentWidget()->setEnabled(true);
}

void Imageproc::on_discard_released()
{
        this->parentWidget()->setEnabled(true);
        this->destroy();
}


void Imageproc::on_apply_released()
{
        emit applyClicked();
        this->parentWidget()->setEnabled(true);
        this->destroy();

}

