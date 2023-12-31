#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QSettings>
#include <QRegularExpression>
#include <QPixmap>
#include <QClipboard>
#include <QDesktopServices>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
//#include <QImageReader>
#include <QLocale>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getDesktopBackgroundFileName();
    QString cleanUpPath(const QString&);


protected:
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void aktualisierungsPulsschlag();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButtonEnglisch_clicked();

    void on_pushButtonDeutsch_clicked();

    void on_pushButtonFranzoesisch_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap desktopBackground;
    void bildLaden();
    void textLaden();
    void graphikenLaden();
    void systemspracheEinstellen();
    QTimer *timer;




};
#endif // MAINWINDOW_H
