#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textLaden();
    bildLaden();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    textLaden();
    bildLaden();
}

QString MainWindow::getDesktopBackgroundFileName() {
    qDebug() << "Funktion getDesktopBackgroundFileName() aufgerufen ";

    // Öffnen der Registry-Schlüssel
    QSettings registrySettings("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);

    //QString zur Aufnahme des neuen Pfades
    QString wallpaperPath = "";

    //Lesen des Werts TranscodedImageCache
    wallpaperPath = registrySettings.value("TranscodedImageCache").toString();

    //Entfernen möglicher Nullterminierungen
    wallpaperPath.remove(QChar('\0'));

    //Bereinigen des Pfads von weiteren unerwünschten Zeichen
    wallpaperPath = cleanUpPath(wallpaperPath);

    //Rückgabe des Pfads
    qDebug() << "Pfad: " << wallpaperPath;
    return wallpaperPath;
}


QString MainWindow::cleanUpPath(const QString &path) {
    //Nur erlaubte Zeichen beibehalten (A-Z, a-z, 0-9, \, :, ., -)
    QString allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZÄÅÁÀÂÇÈÉÊËÎÏÔŒÖÙÛÜẞÐÍÓÚÝŸÞÆØabcdefghijklmnopqrstuvwxyzäáàâåöüßçðéèêëíîïóôœúùûýÿþæø0123456789\\:.-/_, ";

    QString cleanedPath;
    for (const QChar &ch : path) {
        if (allowedChars.contains(ch)) {
            cleanedPath.append(ch);
        }
    }

    return cleanedPath;
}

void MainWindow::on_pushButton_2_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->lineEdit->text());
}


void MainWindow::on_pushButton_3_clicked()
{
    //QString imagePath = getDesktopBackgroundFileName();

    //Nehmen wir lieber den Wert aus dem Label, um Verwirrung zu vermeiden
    QString imagePath = ui->lineEdit->text();

    //Überprüfen, ob der Dateipfad nicht leer ist
    if (!imagePath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath));
    }
}

//Knopf zum Schließen des Programms
void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}

    //Bild in Anzeigetafel laden
void MainWindow::bildLaden(){
    desktopBackground.load(getDesktopBackgroundFileName());
    desktopBackground = desktopBackground.scaled(200, 200, Qt::KeepAspectRatio);
    ui->pushButton_5->setText("");
    ui->pushButton_5->setIcon(QIcon(desktopBackground));
    ui->pushButton_5->setIconSize(desktopBackground.size());
}

//Text in Textfeld laden
void MainWindow::textLaden(){

    ui->lineEdit->setText(getDesktopBackgroundFileName());

    //Wenn der Pfad im Feld ist, aktiviere Knöpfe zum Kopieren und Öffnen
    if (ui->lineEdit->text() != ""){
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
}

