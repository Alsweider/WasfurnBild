#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textLaden();
    bildLaden();

    //Pulsgeber für die automatische Aktualisierung
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(aktualisierungsPulsschlag()));

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

    //Öffnen der Registry-Schlüssel
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
    QString allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZÄÅÁÀÂÃÇÈÉÊËÎÏÔŒÖÕÙÛÜẞÐÍÓÚÝŸÞÆØabcdefghijklmnoõpqrstuvwxyzäáàâåãöüßçðéèêëíîïóôœúùûýÿþæø0123456789\\:.-/_, ";

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
    //Nehmen wir den Wert aus dem Label
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

void MainWindow::aktualisierungsPulsschlag()
{
    textLaden();
    bildLaden();
}

    //Bild in Anzeigetafel laden
void MainWindow::bildLaden(){
    QSize neueGroesse = ui->pushButton_5->size();
    desktopBackground.load(getDesktopBackgroundFileName());
    desktopBackground = desktopBackground.scaled(neueGroesse, Qt::KeepAspectRatio);
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

void MainWindow::resizeEvent(QResizeEvent *event) {
    QSize neueGroesse = ui->pushButton_5->size();
    //Lade das Bild aus dem Textfeld
    desktopBackground.load(ui->lineEdit->text());
    desktopBackground = desktopBackground.scaled(neueGroesse, Qt::KeepAspectRatio);
    ui->pushButton_5->setText("");
    ui->pushButton_5->setIcon(QIcon(desktopBackground));
    ui->pushButton_5->setIconSize(desktopBackground.size());

    QMainWindow::resizeEvent(event); //Rufe die Basisimplementierung auf
}

//Häkchen für die automatische Aktualisierung
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        timer->start(1000);
    } else{
        timer->stop();
    }
}

