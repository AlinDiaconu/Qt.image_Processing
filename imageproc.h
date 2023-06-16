#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "ui_imageproc.h"

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QImage>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

namespace Ui {
class Imageproc;
}

class Imageproc : public QMainWindow
{
    Q_OBJECT

public:
    explicit Imageproc(QLabel *image,QPushButton *buton,QMainWindow *parent = nullptr);
    ~Imageproc();

    Mat Contrast(Mat src,int a,int b,int sa, int sb);
    Mat ImBin(Mat src,int a);
    Mat Negative(Mat src);
    Mat WinOp(Mat src,int a, int b, int r);
    Mat Compressed(Mat src, int c);
    Mat Gamma(Mat src);
    Mat Stralucire(Mat src);
    Mat RGBtoGray(Mat src);
    Mat Histogram(Mat src,float *h);
    Mat EgalizareHisto(Mat src);
    Mat convolutie(Mat src,int m,int n, float w[100][100]);
    Mat MedianFilter(Mat src,int m);
    void Extraction(Mat src);
private slots:
    void on_aValue_valueChanged(int value);
    void on_aValue_Bin(int value);
    void on_bValue_valueChanged(int value);

    void on_aValue_WinOp(int value);
    void on_bValue_WinOp(int value);
    void on_SA_WinOp(const QString &arg3);

    void on_SA_Compress(const QString &arg3);

    void on_SA_textChanged(const QString &arg1);

    void on_SB_textChanged(const QString &arg2);
    void MoveLabel(QSize size, QLabel *label);


    void on_blur_released();

    void on_sharpen_released();

    void on_edgeDet_released();

    void on_median_released();

    void on_discard_released();

    void closeEvent(QCloseEvent *event);

    void on_apply_released();

signals:
    void applyClicked();

private:
    Ui::Imageproc *ui;
    QImage imag;

};

#endif // IMAGEPROC_H
