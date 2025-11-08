#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    progInfoSetzen();
    textLaden();
    systemspracheEinstellen();
    bildLaden();
    //graphikenLaden();

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

//Eingelesenen Registry-Wert von Zeichen außerhalb des eigentlichen Bildpfades bereinigen
QString MainWindow::cleanUpPath(const QString &path) {

    QString cleanedPath;
    bool foundDriveLetter = false;

    for (int i = 0; i < path.length(); ++i) {
        QChar ch = path.at(i);
        //Abfrage ob Laufwerksbuchstabe gefolgt von Doppelpunkt und Rückstrich gefunden wurde
        if (ch.isUpper() && i + 2 < path.length() && path.at(i + 1) == QLatin1Char(':') && path.at(i + 2) == QLatin1Char('\\') && !foundDriveLetter){
            foundDriveLetter = true;
            cleanedPath.clear();
        }

        cleanedPath.append(ch);
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
    Q_UNUSED(arg1);

    if(ui->checkBox->isChecked()){
        timer->start(1000);
    } else{
        timer->stop();
    }
}

// void MainWindow::graphikenLaden(){
//     //Größen für die Flaggen festlegen
//     QSize neueGroesseDeutsch = ui->pushButtonDeutsch->size();
//     QSize neueGroesseFranzoesisch = ui->pushButtonFranzoesisch->size();
//     QSize neueGroesseEnglisch = ui->pushButtonEnglisch->size();


//     //Texte leeren
//     ui->pushButtonDeutsch->setText("");
//     ui->pushButtonFranzoesisch->setText("");
//     ui->pushButtonEnglisch->setText("");

//     //Flaggen hissen und Größen anpassen
//     ui->pushButtonDeutsch->setIcon(QIcon(":/new/bilder/bundesflagge.jpg"));
//     ui->pushButtonDeutsch->setIconSize(neueGroesseDeutsch);

//     ui->pushButtonFranzoesisch->setIcon(QIcon(":/new/bilder/drapeautricolore.jpg"));
//     ui->pushButtonFranzoesisch->setIconSize(neueGroesseFranzoesisch);

//     ui->pushButtonEnglisch->setIcon(QIcon(":/new/bilder/unionjack.jpg"));
//     ui->pushButtonEnglisch->setIconSize(neueGroesseEnglisch);

//     //Flaggen an Knöpfe anpassen
//     ui->pushButtonDeutsch->setStyleSheet("border-image: url(:/new/bilder/bundesflagge.jpg) 0 0 0 0 stretch stretch;");
//     ui->pushButtonFranzoesisch->setStyleSheet("border-image: url(:/new/bilder/drapeautricolore.jpg) 0 0 0 0 stretch stretch;");
//     ui->pushButtonEnglisch->setStyleSheet("border-image: url(:/new/bilder/unionjack.jpg) 0 0 0 0 stretch stretch;");
// }


void MainWindow::on_pushButtonEnglisch_clicked()
{
    ui->pushButton->setText("Read path");
    ui->pushButton->setToolTip("Reads the path of the current desktop wallpaper from the registry.");
    ui->pushButton_2->setText("Copy path");
    ui->pushButton_2->setToolTip("Copies the path to the clipboard.");
    ui->pushButton_3->setText("Open picture");
    ui->pushButton_3->setToolTip("Opens the image in the system image viewer.");
    ui->pushButton_4->setText("Quit");
    ui->pushButton_4->setToolTip("Closes the programme.");
    //ui->pushButton_5->setText("Image");
    ui->pushButton_5->setToolTip("Click to open image in system image viewer.");
    ui->checkBox->setText("Reload automatically");
    ui->checkBox->setToolTip("If activated, the programme automatically reads the current background image every second.");
    qDebug() << "Englisch festgelegt";
}


void MainWindow::on_pushButtonDeutsch_clicked()
{
        ui->pushButton->setText("Pfad auslesen");
        ui->pushButton->setToolTip("Liest den Pfad des aktuellen Desktop-Hintergrundbildes aus der Registry.");
        ui->pushButton_2->setText("Pfad kopieren");
        ui->pushButton_2->setToolTip("Kopiert den Pfad in die Zwischenablage.");
        ui->pushButton_3->setText("Bild öffnen");
        ui->pushButton_3->setToolTip("Öffnet das Bild im System-Bildbetrachter.");
        ui->pushButton_4->setText("Beenden");
        ui->pushButton_4->setToolTip("Schließt das Programm.");
        //ui->pushButton_5->setText("Bild");
        ui->pushButton_5->setToolTip("Klicken um Bild im System-Bildbetrachter zu öffnen.");
        ui->checkBox->setText("Kontinuierlich abgleichen");
        ui->checkBox->setToolTip("Wenn aktiviert, liest das Programm das aktuelle Hintergrundbild selbständig im Sekundentakt ein.");
        qDebug() << "Deutsch festgelegt";
}


void MainWindow::on_pushButtonFranzoesisch_clicked()
{
    ui->pushButton->setText("Lire le chemin");
    ui->pushButton->setToolTip("Lit le chemin du fond d'écran actuel à partir du registre.");
    ui->pushButton_2->setText("Copier le chemin");
    ui->pushButton_2->setToolTip("Copie le chemin dans le presse-papiers.");
    ui->pushButton_3->setText("Ouvrir l'image");
    ui->pushButton_3->setToolTip("Ouvre l'image dans la visionneuse d'images système.");
    ui->pushButton_4->setText("Quitter");
    ui->pushButton_4->setToolTip("Ferme le programme.");
    //ui->pushButton_5->setText("Image");
    ui->pushButton_5->setToolTip("Cliquez pour ouvrir l'image dans la visionneuse d'images système.");
    ui->checkBox->setText("Synchroniser automatiquement");
    ui->checkBox->setToolTip("Lorsqu'il est activé, le programme lit automatiquement l'arrière-plan actuel à intervalles d'une seconde.");
    qDebug() << "Französisch festgelegt";
}

void MainWindow::systemspracheEinstellen(){
    qDebug() << "Spracheinstellung aufgerufen";
    QLocale systemLocale = QLocale::system();

    if (systemLocale.language() == QLocale::German) {
        on_pushButtonDeutsch_clicked();
    } else if (systemLocale.language() == QLocale::English) {
        on_pushButtonEnglisch_clicked();
    } else if (systemLocale.language() == QLocale::French){
        on_pushButtonFranzoesisch_clicked();
    } else {
        qDebug() << "Systemsprache nicht verfügbar";
        on_pushButtonDeutsch_clicked();
    }
}


void MainWindow::on_comboBoxSprache_currentIndexChanged(int index)
{
    if (index == 0){
        on_pushButtonDeutsch_clicked();
    } else if (index == 1){
        on_pushButtonFranzoesisch_clicked();
    } else if (index == 2){
        on_pushButtonEnglisch_clicked();
    } else {
        return;
    }
}


void MainWindow::progInfoSetzen(){
    QCoreApplication::setOrganizationName("Alsweider");
    QCoreApplication::setOrganizationDomain("https://github.com/Alsweider");
    QCoreApplication::setApplicationName("WasfurnBild");
    QCoreApplication::setApplicationVersion(QString(APP_VERSION));
    this->setWindowTitle(QString("%1 v%2").arg(APP_NAME, APP_VERSION));
}

