#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QMainWindow>
#include <QLabel>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
protected slots:
    void EditPressed();
    void on_actionOpenFile_triggered();
    void MoveLabel(QSize size, QLabel *label);
    void applyClicked();

public slots:
    void NewWindow();
private slots:
    void on_actionSave_file_triggered();
};
#endif // MAINWINDOW_H
