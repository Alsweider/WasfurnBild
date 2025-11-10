#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QRegularExpression>
#include <QProcess>
#include <QTimer>
#include <functional>
#include <QImage>
#include <QTemporaryFile>
#include <QStandardPaths>



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


void MainWindow::setzeSpracheEnglisch()
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
    ui->comboBoxSprache->setToolTip("Language");
    ui->pushButtonSuchmaschine->setText("Web Search");
    ui->pushButtonSuchmaschine->setToolTip("The image is temporarily uploaded to litterbox.catbox.moe via cURL "
                                           "in order to perform an image search on tineye.com. After one hour, "
                                           "the image is deleted from Litterbox.");
    ui->lineEditLitterboxURL->setPlaceholderText("Litterbox URL");
    ui->lineEditLitterboxURL->setToolTip("URL of the uploaded image (available for 1 hour)");
    ui->checkBoxNichtFragen->setText("Do not ask");
    ui->checkBoxNichtFragen->setToolTip("Disable safety prompt before upload");
    qDebug() << "Englisch festgelegt";
}


void MainWindow::setzeSpracheDeutsch()
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
        ui->comboBoxSprache->setToolTip("Sprache");
        ui->pushButtonSuchmaschine->setText("Suche im Netz");
        ui->pushButtonSuchmaschine->setToolTip("Lädt das Bild via cURL temporär auf litterbox.catbox.moe hoch, "
                                               "um eine Bildersuche über tineye.com durchzuführen. Nach 1 Stunde "
                                               "wird das Bild von Litterbox wieder gelöscht.");
        ui->lineEditLitterboxURL->setPlaceholderText("Litterbox-URL");
        ui->lineEditLitterboxURL->setToolTip("URL zum hochgeladenen Bild (1 Stunde verfügbar)");
        ui->checkBoxNichtFragen->setText("Nicht mehr fragen");
        ui->checkBoxNichtFragen->setToolTip("Sicherheitsfrage vor dem Hochladen abschalten");

        qDebug() << "Deutsch festgelegt";
}


void MainWindow::setzeSpracheFranzoesisch()
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
    ui->comboBoxSprache->setToolTip("Langue");
    ui->pushButtonSuchmaschine->setText("Recherche Net");
    ui->pushButtonSuchmaschine->setToolTip("L'image est temporairement téléchargée sur litterbox.catbox.moe "
                                           "via cURL afin d'effectuer une recherche d'image sur tineye.com. "
                                           "Après 1 heure, l'image est supprimée de Litterbox.");
    ui->lineEditLitterboxURL->setPlaceholderText("URL Litterbox");
    ui->lineEditLitterboxURL->setToolTip("URL de l'image téléchargée (disponible pendant 1 heure)");
    ui->checkBoxNichtFragen->setText("Ne plus demander");
    ui->checkBoxNichtFragen->setToolTip("Désactiver la demande de confirmation avant le téléchargement");

    qDebug() << "Französisch festgelegt";
}

void MainWindow::systemspracheEinstellen(){
    qDebug() << "Spracheinstellung aufgerufen";
    QLocale systemLocale = QLocale::system();

    if (systemLocale.language() == QLocale::German) {
        setzeSpracheDeutsch();
    } else if (systemLocale.language() == QLocale::English) {
        setzeSpracheEnglisch();
    } else if (systemLocale.language() == QLocale::French){
        setzeSpracheFranzoesisch();
    } else {
        qDebug() << "Systemsprache nicht verfügbar";
        setzeSpracheDeutsch();
    }
}


void MainWindow::on_comboBoxSprache_currentIndexChanged(int index)
{
    if (index == 0){
        setzeSpracheDeutsch();
    } else if (index == 1){
        setzeSpracheFranzoesisch();
    } else if (index == 2){
        setzeSpracheEnglisch();
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


void MainWindow::on_pushButtonSuchmaschine_clicked()
{
    //Sicherheitsfrage vor dem Hochladen
    if (ui->checkBoxNichtFragen->isChecked()){
        bilderSuche();
        return;
    }

    // Texte für alle Sprachen definieren
    struct MessageTexts {
        const char* title;
        const char* text;
        const char* yesButton;
        const char* noButton;
    };

    // 0 = Deutsch, 1 = Französisch, 2 = Englisch
    const MessageTexts messages[3] = {
        { "Bildsuche starten?",
         "Das Bild wird temporär auf den Bilderdienst litterbox.catbox.moe hochgeladen, "
         "wobei auch Ihre IP-Adresse übermittelt wird.\n"
         "Das ist nötig, da TinEye.com ein im Netz verfügbares Bild erwartet, "
         "um die Bildsuche starten zu können.\n"
         "Nach 1 Stunde wird das Bild von Catbox wieder gelöscht. TinEye selbst versichert, "
         "keine Bilder zu speichern.\n\n"
         "Möchten Sie die Bildsuche starten?",
         "Ja", "Nein" },

        { "Lancer la recherche d'image ?",
         "L'image sera temporairement téléchargée sur le service d'images litterbox.catbox.moe, "
         "et votre adresse IP sera également transmise.\n"
         "Cela est nécessaire car TinEye.com attend une image disponible en ligne "
         "pour pouvoir lancer la recherche.\n"
         "Après 1 heure, l'image sera supprimée de Catbox. TinEye assure qu'il ne stocke aucune image.\n\n"
         "Voulez-vous lancer la recherche d'image ?",
         "Oui", "Non" },

        { "Start image search?",
         "The image will be temporarily uploaded to the image service litterbox.catbox.moe, "
         "and your IP address will also be transmitted.\n"
         "This is necessary because TinEye.com expects an image available online "
         "to perform the image search.\n"
         "After 1 hour, the image will be deleted from Catbox. TinEye itself assures "
         "that it does not store any images.\n\n"
         "Do you want to start the image search?",
         "Yes", "No" }
    };

    // Sprache auswählen
    int langIndex = ui->comboBoxSprache->currentIndex();
    if (langIndex < 0 || langIndex > 2){
        langIndex = 0; // Standard Deutsch
    }

    const auto& msg = messages[langIndex];

    // MessageBox mit eigenen Buttons
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr(msg.title));
    msgBox.setText(tr(msg.text));

    QPushButton* yesButton = msgBox.addButton(tr(msg.yesButton), QMessageBox::YesRole);
    QPushButton* noButton  = msgBox.addButton(tr(msg.noButton), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        bilderSuche();
    }
}


void MainWindow::bilderSuche(){
    ui->pushButtonSuchmaschine->setEnabled(false);
    ui->statusBar->showMessage("Bildsuche gestartet...");

    // RAII-Schutz: stellt sicher, dass die Schaltfläche beim Verlassen wieder aktiviert wird
    auto buttonGuard = qScopeGuard([this]() {
        ui->pushButtonSuchmaschine->setEnabled(true);
        qDebug() << "Suchschaltfläche wieder aktiviert.";
    });

    QString imagePath = ui->lineEdit->text().trimmed();
    imagePath.remove('"');
    qDebug() << "Pfad zum Bild: " << imagePath;

    // Tineye kann jfif nicht verarbeiten, also eine Temp-Kopie als JPEG speichern
    QString tempPath = convertJfifToJpeg(imagePath);

    if (tempPath.isEmpty()) {
        QMessageBox::warning(this, "Fehler", "Kein Bildpfad vorhanden.");
        return;
    } else {
        qDebug() << "Bildpfad gefunden (tempPath): " << tempPath;
    }

    QFileInfo fi(tempPath);
    if (!fi.exists() || !fi.isFile()) {
        QMessageBox::warning(this, "Fehler", "Bilddatei existiert nicht.");
        return;
    } else {
        qDebug() << "Lokales Bild gefunden: " << fi;
    }

    QStringList arguments;
    arguments << "-F" << "reqtype=fileupload"
              << "-F" << "time=1h"
              << "-F" << QString("fileToUpload=@%1").arg(tempPath)
              << "https://litterbox.catbox.moe/resources/internals/api.php";
    qDebug() << "cURL zusammengestellt: " << arguments;

    QProcess *curlProcess = new QProcess(this);

    connect(curlProcess, &QProcess::finished, this,
            [this, curlProcess, tempPath, buttonGuard = std::move(buttonGuard)]() mutable {
                QByteArray output = curlProcess->readAllStandardOutput().trimmed();
                QByteArray error = curlProcess->readAllStandardError();
                curlProcess->deleteLater();

                if (output.isEmpty()) {
                    QMessageBox::warning(this, "Fehler", "Upload fehlgeschlagen:\n" + QString(error));
                    qDebug() << "Upload fehlgeschlagen: " << error;
                    return;
                }

                // QString url = QString::fromUtf8(output);
                QString url = QString::fromUtf8(output).trimmed();

                // Ist das überhaupt eine URL?
                if (!url.startsWith("http")) {
                    QString shortMsg = url.left(200); // Nur die ersten 200 Zeichen für Anzeige, sonst kommt die ganze Verwandtschaft
                    if (url.length() > 200)
                        shortMsg += "...";

                    QMessageBox::warning(
                        this,
                        "Fehler",
                        "Upload fehlgeschlagen oder keine gültige URL erhalten.\n\n"
                        "Serverantwort (gekürzt):\n" + shortMsg
                        );

                    qDebug() << "Ungültige Upload-Antwort (vollständig):" << url;
                    return;
                }

                ui->lineEditLitterboxURL->setText(url);
                ui->lineEditLitterboxURL->setEnabled(true);
                ui->statusBar->showMessage(QString("Upload abgeschlossen: %1").arg(url));
                qDebug() << "Upload abgeschlossen: " << url;

                QUrl qurl(url);
                QString urlCopy = url;

                // Lambda zur wiederholten Prüfung der Verfügbarkeit
                std::shared_ptr<std::function<void()>> checkAvailability = std::make_shared<std::function<void()>>();

                int maxAttempts = 10;
                int delayMs = 500; // Startwert für die Wartezeit
                int maxDelayMs = 6000;
                int attemptCount = 0;

                *checkAvailability = [this, qurl, urlCopy, checkAvailability, maxAttempts, &attemptCount, delayMs, maxDelayMs]() mutable {
                    if (attemptCount >= maxAttempts) {
                        ui->statusBar->showMessage("Bild nach mehreren Versuchen nicht erreichbar.");
                        qDebug() << "Maximale Anzahl der Prüfungen erreicht.";
                        return;
                    }
                    attemptCount++;
                    qDebug() << "Verfügbarkeitsprüfung Versuch:" << attemptCount;

                    QNetworkRequest request(qurl);
                    QNetworkReply *reply = m_networkManager.head(request);

                    connect(reply, &QNetworkReply::finished, this, [this, reply, urlCopy, checkAvailability, &attemptCount, &delayMs, maxDelayMs, maxAttempts]() mutable {
                        if (reply->error() == QNetworkReply::NoError) {
                            qDebug() << "Netzwerkantwort erhalten, Bild erreichbar.";
                            QString tineyeUrl = QString("https://tineye.com/search?url=%1")
                                                    .arg(QUrl::toPercentEncoding(urlCopy));
                            QDesktopServices::openUrl(QUrl(tineyeUrl));
                            ui->statusBar->showMessage("Tineye-Suche geöffnet.", 7000);
                            qDebug() << "Tineye geöffnet";
                        } else {
                            QString statusMsg = QString("Prüfe Erreichbarkeit des Bildes erneut... Versuch %1/%2").arg(attemptCount, maxAttempts);
                            ui->statusBar->showMessage(statusMsg);
                            qDebug() << "Fehler beim Zugriff, erneute Prüfung in" << delayMs << "ms";
                            // Exponentielles Backoff: Verzögerung verdoppeln, bis zu 6 Sekunden
                            delayMs = qMin(delayMs * 2, maxDelayMs);
                            QTimer::singleShot(delayMs, this, [checkAvailability]() { (*checkAvailability)(); });
                        }
                        reply->deleteLater();
                    });
                };

                // Start der ersten Prüfung sofort
                (*checkAvailability)();

                // Temporäre JPEG-Kopie löschen, falls vorhanden
                QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
                if (tempPath.startsWith(tempDir)) {
                    QFile::remove(tempPath);
                    qDebug() << "Temporäre JPEG-Datei gelöscht:" << tempPath;
                } else {
                    qDebug() << "Kein temporäres Bild, nichts gelöscht:" << tempPath;
                }
            });

    curlProcess->start("curl", arguments);
}


//Nicht unterstütztes Format für die Suchmaschine temporär umwandeln
QString MainWindow::convertJfifToJpeg(const QString &path)
{
    QFileInfo fi(path);
    if (fi.suffix().compare("jfif", Qt::CaseInsensitive) != 0)
        return path; // Keine JFIF-Datei, keine Umwandlung

    QImage image(path);
    if (image.isNull()) {
        qDebug() << "Bild konnte nicht geladen werden:" << path;
        return path;
    }

    // Temporäre Datei im System-Temp-Ordner erzeugen
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QTemporaryFile tempFile(tempDir + "/temp_jfif_XXXXXX.jpg");
    tempFile.setAutoRemove(false); // Wir löschen selbst nach Upload

    if (!tempFile.open()) {
        qDebug() << "Temporäre Datei konnte nicht erzeugt werden.";
        return path;
    }

    QString tempPath = tempFile.fileName();
    tempFile.close();

    if (image.save(tempPath, "JPG")) {
        qDebug() << "JFIF temporär in JPEG umgewandelt:" << tempPath;
        return tempPath;
    } else {
        qDebug() << "Fehler beim Speichern der temporären JPEG-Datei.";
        return path;
    }
}
