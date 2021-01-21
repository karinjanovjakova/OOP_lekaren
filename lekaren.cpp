#include "lekaren.h"
//chyba: zmena existujuceho zakaznika na premium 

lekaren::lekaren(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    start();
}

void lekaren::ReloadComboBox() {
    ui.comboBox->clear();
    int i, index;
    for (i = 0; i < objednavky.size(); i++) {
        ui.comboBox->addItem(objednavky[i]->getlogin());
    }
}

void lekaren:: ReloadComboUsers () {
    ui.UsersComboBox->clear();
    int i, index;
    for (i = 0; i < loginy.size(); i++) {
        ui.UsersComboBox->addItem(loginy[i]->getlogin());
    }
}

void lekaren:: fill_table_admin() {     //vypise nazvy liekov so sklad. zasobami
    ui.table->setRowCount(lieky.size());
    ui.table->setColumnCount(2);
    int i;
    for (i = 0; i < lieky.size(); i++) {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(lieky[i]->getnazov());
        QTableWidgetItem* item3 = new QTableWidgetItem;
        item3->setText(QString::number(sklad[i]));

        ui.table->setItem(i, 0, item);
        ui.table->setItem(i, 1, item3);
    }
}

void lekaren::fill_table_zak() {        //vypise nazvy liekov s cenami
    ui.table->clear();
    ui.table->setRowCount(lieky.size());
    ui.table->setColumnCount(2);
    int i,a;
    float cena;
    for (i = 0; i < lieky.size(); i++) {
        if (sklad[i] > 0) {
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(lieky[i]->getnazov());
            if (prihlaseny->gettyp() == 3) {
                cena = lieky[i]->getcena() * (100 - prihlaseny->getzlava()) * 0.01;
            }
            else
                cena = lieky[i]->getcena();
            QTableWidgetItem* item2 = new QTableWidgetItem;
            item2->setText(QString::number(cena));

            ui.table->setItem(i, 0, item);
            ui.table->setItem(i, 1, item2);
        }
    }
}

void warning(QString text) {
    QMessageBox msgWarning;
    msgWarning.setText(text);
    msgWarning.setIcon(QMessageBox::Critical);
    msgWarning.setWindowTitle("Caution");
    msgWarning.exec();
}

void info(QString text) {
    QMessageBox msgWarning;
    msgWarning.setText(text);
    msgWarning.setIcon(QMessageBox::Information);
    msgWarning.setWindowTitle("Information");
    msgWarning.exec();
}

void lekaren :: pouzivatelia() {
    loginy.clear();
    QFile users("doc.txt");             
    if (!users.open(QIODevice::ReadOnly)) {  
        warning(u8"Nepodarilo sa otvoriù s˙bor s uûÌvateæmi.");
    }
   
    while (!users.atEnd()) {
         QString line = users.readLine();       
         int ID = (line.split(',').at(0).toInt());
         QString meno = (line.split(',').at(1));
         QString heslo = (line.split(',').at(2));
         if (ID == 0) {
             loginy.push_back(new admin(ID, meno, heslo));
         }
         if (ID == 1) {
             QString poz = (line.split(',').at(3));
             loginy.push_back(new staff(ID, meno, heslo, poz));
         }
         if (ID == 2) {
             QString name = (line.split(',').at(3));
             QString priez = (line.split(',').at(4));
             QString adr = (line.split(',').at(5));
             float min = (line.split(',').at(6).toFloat());
             if (min > 100) {
                 ID = 3;
             }
             else if (min<=100)
                loginy.push_back(new zakaznik(ID, meno, heslo, name, priez, adr, min));
         }
         if (ID == 3) {
             QString name = (line.split(',').at(3));
             QString priez = (line.split(',').at(4));
             QString adr = (line.split(',').at(5));
             float min = (line.split(',').at(6).toFloat());
             loginy.push_back(new premium(ID, meno, heslo, name, priez, adr, min));
         }

     }
    users.close();
}

void lekaren::liekyload(){
    lieky.clear();
    sklad.clear();
    QFile file("lieky.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        warning(u8"Nepodarilo sa otvoriù s˙bor s liekmi");
    }
    while (!file.atEnd()) {
        QString line = file.readLine();
        QString name = (line.split(',').at(0));
        float cena = (line.split(',').at(1).toFloat());
        int ks = (line.split(',').at(2).toInt());
        lieky.push_back(new liek(name, cena));
        sklad.append(ks);
    }
    file.close();
}

QString lekaren::encrypt(QString heslo)
{
    QString zakodovane = QCryptographicHash::hash(heslo.toStdString().c_str(), QCryptographicHash::Sha1).toHex();
    return zakodovane;
}

void lekaren::start() {            //pociatocne podmienky ui

    pouzivatelia();
    liekyload();
    objednavkyload();
    ui.groupBox->setVisible(true);
    ui.groupBox_2->setVisible(false);
    ui.groupBox_3->setVisible(false);
    ui.groupBox_4->setVisible(false);
    ui.groupBox_5->setVisible(false);
    ui.login->setDisabled(false);
    ui.logout->setDisabled(true);
    ui.actionFile->setDisabled(true);
    ui.actionTxt->setDisabled(true);
    ui.User->setDisabled(true);
    ui.groupBox_3->setTitle(u8"N·kup");
    ui.rem2->setText(u8"Odstr·niù");
    ui.lineEditName->setDisabled(false);
    ui.lineEditPswd->setDisabled(false);
    ui.lineEditName->setText("");
    ui.lineEditPswd->setText("");
    ui.kosik->clear();
    ui.listWidget->clear();
    ui.checkBox->setChecked(false);
    ReloadComboUsers();
    ReloadComboBox();
}

void lekaren::on_login_clicked()     //prihlasenie, uprava ui podla uzivatela
{

    QString meno = ui.lineEditName->text();
    QString heslo = ui.lineEditPswd->text();
    int i=0, index=0;
    bool sedi=false, spravnemeno=false;
    for (i = 0; i < loginy.size(); i++) {
        
        if (meno == loginy[i]->getlogin()) {
            spravnemeno = true;
            if (encrypt(heslo) == loginy[i]->getheslo()) {
                sedi = true;
                index = i;
            }
            else {
                warning(u8"ZadanÈ heslo je nespr·vne.");
                sedi = false;
            }
        }
    }
    if (!spravnemeno) 
        warning(u8"Nespr·vne meno alebo heslo.");
    if (sedi) {
        prihlaseny = loginy[index];
        ui.login->setDisabled(true);
        ui.logout->setDisabled(false);
        ui.lineEditName->setDisabled(true);
        ui.lineEditPswd->setDisabled(true);
        ui.groupBox_5->setVisible(true);
        ui.groupBox_6->setVisible(true);
        ui.groupBox_7->setVisible(false);
        ui.groupBox_8->setVisible(false);
        ui.groupBox_9->setVisible(false);
        ui.groupBox_10->setVisible(false);
        if (objednavky.isEmpty()) {
            qDebug() << "vykonavam";
            ui.reload->setDisabled(true);
            ui.rem2->setDisabled(true);
        }
        if (loginy[index]->gettyp() == 0) {                 //admin
            ui.actionFile->setDisabled(false);
            ui.User->setDisabled(false);
            ui.groupBox_3->setTitle(u8"Aktu·lny sklad");
            ui.groupBox_2->setVisible(false);
            ui.groupBox_3->setVisible(true);
            ui.groupBox_4->setVisible(false);
            ui.Add->setVisible(false);
            ui.UsersComboBox->setDisabled(false);
            fill_table_admin();
        }
        else if (loginy[index]->gettyp() == 1) {            //staff
            ui.rem2->setText(u8"Potvrdiù");
            ui.groupBox_3->setVisible(false);
            ui.groupBox_4->setVisible(true);
            ui.groupBox_2->setVisible(false);
            ui.UsersComboBox->setDisabled(true);
            ui.actionTxt->setDisabled(false);
            if (!objednavky.isEmpty())
                ui.rem2->setDisabled(false);
        }
        else if (loginy[index]->gettyp() == 2) {            //zakaznik
            ui.Add->setDisabled(false);
            ui.rem->setDisabled(false);
            ui.pay->setDisabled(false);
            ui.groupBox->setVisible(true);
            ui.groupBox_2->setVisible(true);
            ui.groupBox_3->setVisible(true);
            ui.groupBox_4->setVisible(true);
            ui.Add->setVisible(true);
            ui.UsersComboBox->setDisabled(true);
            if (!objednavky.isEmpty() && objednavky[ui.comboBox->currentIndex()]->getlogin() != prihlaseny->getlogin())
                ui.rem2->setDisabled(true);
            fill_table_zak();
        }
        else if (loginy[index]->gettyp() == 3) {            //premium
            ui.Add->setDisabled(false);
            ui.rem->setDisabled(false);
            ui.pay->setDisabled(false);
            ui.groupBox->setVisible(true);
            ui.groupBox_2->setVisible(true);
            ui.groupBox_3->setVisible(true);
            ui.groupBox_4->setVisible(true);
            ui.Add->setVisible(true);
            ui.UsersComboBox->setDisabled(true);
            if (!objednavky.isEmpty() && objednavky[ui.comboBox->currentIndex()]->getlogin() != prihlaseny->getlogin())
                ui.rem2->setDisabled(true);
            fill_table_zak();
        }
        else {
            warning("Neexistuj˙ci pouûÌvateæ.");
            start();
        }
    }
}

void lekaren::on_logout_clicked(){     //navrat na pociatocne ui po odhlaseni
    zapisLieky();
    zapisObjednavky();
    start(); 
    prihlaseny->clearkos();
    prihlaseny->setsuma(0);
    info(u8"NepotvrdenÈ zmeny/n·kupy boli zmazanÈ."); 
    zapisUsers();
}

void lekaren::on_Add_clicked(){        // pridanie do kosika, doriesit skladove zasoby
    QModelIndex index = ui.table->currentIndex();
    qDebug() << index.row();
    if (index.row() < 0) 
        warning("Vyberte produkt.");  
    else if (sklad[index.row()]==0)
        warning(u8"ºutujeme, produkt uû nie je na sklade.");
    else if (sklad[index.row()] > 0) {
        sklad[index.row()]--;
        prihlaseny->append(index.row());
        if (prihlaseny->gettyp() == 2)
            prihlaseny->sumaplus(lieky[index.row()]->getcena());
        else if (prihlaseny->gettyp() == 3)
            prihlaseny->sumaplus(lieky[index.row()]->getcena() * (100 - prihlaseny->getzlava()) * 0.01);
    }
    fill_table_zak();
    reloadkos();
}

void lekaren::reloadkos() {      //znovu nacita kosik
    int i, j;
    ui.kosik->clear();
    for (i = 0; i < prihlaseny->sizekos(); i++) {
        QListWidgetItem* liek = new QListWidgetItem;
        liek->setText(lieky[prihlaseny->getindex(i)]->getnazov());
        ui.kosik->addItem(liek);
    }
}

void lekaren::on_rem_clicked(){        //odstrani polozku z kosika, vrati do skladu
    QModelIndex index = ui.kosik->currentIndex();
    if (!prihlaseny->getkos().empty() && prihlaseny->getkos().size() > index.row() && index.row() >= 0) {
        if (prihlaseny->gettyp() == 2)
            prihlaseny->sumaminus(lieky[prihlaseny->getindex(index.row())]->getcena());
        else if (prihlaseny->gettyp() == 3)
            prihlaseny->sumaminus(lieky[prihlaseny->getindex(index.row())]->getcena() * (100 - prihlaseny->getzlava()) * 0.01);
        sklad[prihlaseny->getindex(index.row())] = sklad[prihlaseny->getindex(index.row())] + 1;
        prihlaseny->odober(index.row());
        qDebug() << prihlaseny->getkos() << sklad;
    }
    else if (!prihlaseny->getkos().empty() && prihlaseny->getkos().size() <= index.row())
        warning(u8"T·to poloûka uû nie je v koöÌku, stlaËte znovu naËÌtaù.");
    else if (index.row() < 0 && !prihlaseny->getkos().empty())
        warning(u8"Zvoæte poloûku.");
    else
        warning(u8"KoöÌk je pr·zdny, najskÙr nieËo pridajte.");
    fill_table_zak();
    reloadkos();
}

void lekaren::on_pay_clicked() {        //spravi z kosika objednavku, do comboboxu nacita objednavky
    if (prihlaseny->getkos().empty())
        warning(u8"KoöÌk je pr·zdny.");
    else {
        objednavka* obj = new objednavka (prihlaseny->getsuma(), prihlaseny->getlogin(), prihlaseny->getkos());
        info(u8"Objedn·vka bola ˙speöne odoslan·. Po potvrdenÌ objedn·vky V·m zmizne z pola objednanÈ a obdrûÌte SMS notifik·ciu.");
        objednavky.append(obj);
        prihlaseny->clearkos();
        prihlaseny->setsuma(0);
        ui.listWidget->clear();
        ui.comboBox->clear();
        ui.reload->setDisabled(false);
        ui.rem2->setDisabled(false);
        
    }   
    ReloadComboBox();
    reloadkos();
}

void lekaren::on_reload_clicked() {     //vypise objednavku do listwidgetu
    int index,i;
    ui.listWidget->clear();
    index = ui.comboBox->currentIndex();
    if (prihlaseny->gettyp() == 1) {
        for (i = 0; i < objednavky[index]->getkos().size(); i++) {
            qDebug() << index;
            ui.listWidget->addItem(lieky[objednavky[index]->getindex(i)]->getnazov());
        }
    }
    if (prihlaseny->gettyp() == 2 || prihlaseny->gettyp() == 3) {
        if (ui.comboBox->currentText() == prihlaseny->getlogin()){
            for (i = 0; i < objednavky[index]->getkos().size(); i++) {
                qDebug() << index;
                ui.listWidget->addItem(lieky[objednavky[index]->getindex(i)]->getnazov());
            }
        }
    }
}

void lekaren::on_rem2_clicked() {               //remove pre customera, potvrdit pre staff (zmazanie objednavky, zapisanie do suboru)
    int index, i, j;
    if (prihlaseny->gettyp() == 1) {        //staff
        index = ui.comboBox->currentIndex();
        QString a = ui.comboBox->currentText();
        qDebug() << index;
        a += ".txt";
        qDebug() << a;
        QFile file(a);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            warning(u8"Subor sa neotvoril spravne.");
        }
        else {                                 
            for (i = 0; i < loginy.size(); i++) {                   //toto asi nefunguje
                if (loginy[i]->getlogin() == objednavky[index]->getlogin()) {
                    loginy[i]->setminute(loginy[i]->getminute() + objednavky[index]->getsum());
                    qDebug() << "priratavam sumu";
                }
            }
            QTextStream out(&file);
            out << "Objednavka pouzivatela " << objednavky[index]->getlogin() << "\n";
            for (i = 0; i < objednavky[index]->getkos().size(); i++) {
                j = objednavky[index]->getindex(i);
                sklad[j]++;
                out  << lieky[j]->getnazov() << "\n";
            }
            out << u8"Suma objednavky:" << objednavky[index]->getsum() << "\n";
            out << "***************************************\n";
        }
        objednavky.remove(index);
        qDebug() << objednavky;
        ReloadComboBox();
        info(u8"Objedn·vka bola potvrden· a zapÌsan· do s˙boru.");
    }
    if (prihlaseny->gettyp() == 2 || prihlaseny->gettyp() == 3 && !objednavky.isEmpty()) {          //zakaznik
        index = ui.comboBox->currentIndex();
        if (objednavky[index]->getlogin() != prihlaseny->getlogin())
            warning(u8"Nie je moûnÈ zmazaù objedn·vku.");
        else {
            for (i = 0; i < objednavky[index]->getkos().size(); i++) {
                j = objednavky[index]->getindex(i);
                sklad[j]++;
            }
            objednavky.remove(index);
            ReloadComboBox();
            fill_table_zak();
            info(u8"Objedn·vka bola ˙speöne odstr·nen·");
        }
    } 
    if (objednavky.isEmpty()) {
        ui.reload->setDisabled(true);
        ui.rem2->setDisabled(true);
    }
    ui.listWidget->clear();
}

void lekaren::on_checkBox_clicked() {
    if (ui.checkBox->isChecked()) {//zobraz_podla_uzivatela
        if (prihlaseny->gettyp() == 0) {        //admin musi najskor vybrat z comboboxu
            ui.UsersComboBox->setDisabled(true);
            int index,typ;
            index = ui.UsersComboBox->currentIndex();
            typ = loginy[index]->gettyp();
            if (typ == 0) {
                ui.groupBox_7->setVisible(true);
                ui.groupBox_10->setVisible(true);
                ui.login_2->setText(loginy[index]->getlogin());
                ui.heslo->setText(loginy[index]->getheslo());
            }
            if (typ == 1) {
                ui.groupBox_7->setVisible(true);
                ui.groupBox_9->setVisible(true);
                ui.groupBox_10->setVisible(true);
                ui.login_2->setText(loginy[index]->getlogin());
                ui.heslo->setText(loginy[index]->getheslo());
                ui.pozicia->setText(loginy[index]->getpoz());
            }
            if (typ==2 || typ==3){
                ui.groupBox_7->setVisible(true);
                ui.groupBox_8->setVisible(true);
                ui.groupBox_10->setVisible(true);
                ui.login_2->setText(loginy[index]->getlogin());
                ui.heslo->setText(loginy[index]->getheslo());
                ui.meno->setText(loginy[index]->getmeno());
                ui.priezvisko->setText(loginy[index]->getpriezvisko());
                ui.adresa->setText(loginy[index]->getadresa());
            }
        }
        else if (prihlaseny->gettyp() == 1) {
            ui.groupBox_9->setVisible(true);
            ui.groupBox_10->setVisible(true);
            ui.pozicia->setText(prihlaseny->getpoz());
        }
        else if (prihlaseny->gettyp() == 2 || prihlaseny->gettyp() == 3) {
            ui.groupBox_8->setVisible(true);
            ui.groupBox_10->setVisible(true);
            ui.login_2->setText(prihlaseny->getlogin());
            ui.heslo->setText(prihlaseny->getheslo());
            ui.meno->setText(prihlaseny->getmeno());
            ui.priezvisko->setText(prihlaseny->getpriezvisko());
            ui.adresa->setText(prihlaseny->getadresa());
        }
    }
    else {
        ui.groupBox_7->setVisible(false);
        ui.groupBox_8->setVisible(false);
        ui.groupBox_9->setVisible(false);
        ui.groupBox_10->setVisible(false);
        if (prihlaseny->gettyp()==0)
            ui.UsersComboBox->setDisabled(false);
    }
}

void lekaren::on_Save_clicked() {
    int index, typ, i;
    if (prihlaseny->gettyp() == 0){
        index = ui.UsersComboBox->currentIndex();
        typ=loginy[index]->gettyp();
        if (typ == 0) {
            loginy[index]->setlogin(ui.login_2->text());
            loginy[index]->setheslo(encrypt(ui.heslo->text()));
        }
        else if (typ == 1) {
            loginy[index]->setlogin(ui.login_2->text());
            loginy[index]->setheslo(encrypt(ui.heslo->text()));
            loginy[index]->setpoz(ui.pozicia->text());
        }
        else if (typ == 2 || typ == 3) {
            loginy[index]->setlogin(ui.login_2->text());
            loginy[index]->setheslo(encrypt(ui.heslo->text()));
            loginy[index]->setmeno(ui.meno->text());
            loginy[index]->setpriezvisko(ui.priezvisko->text());
            loginy[index]->setadresa(ui.adresa->text());
        }
    }
    else if (prihlaseny->gettyp() == 1){
        for (i = 0; i < loginy.size(); i++) {
            if (prihlaseny->getlogin() == loginy[i]->getlogin()) {
                index = i;
            }
            loginy[index]->setpoz(ui.pozicia->text());
        }

    }
    else if (prihlaseny->gettyp() == 2 || prihlaseny->gettyp() == 3){
        for (i = 0; i < loginy.size(); i++) {
            if (prihlaseny->getlogin() == loginy[i]->getlogin()) {
                index = i;
            }
        }
        loginy[index]->setmeno(ui.meno->text());
        loginy[index]->setpriezvisko(ui.priezvisko->text());
        loginy[index]->setadresa(ui.adresa->text());
    }
    zapisUsers();
}

void lekaren::zapisUsers() {     //zapise loginy do suboru, zavola sa pri uloûenÌ ˙prav v mainwindow a pri uloûenÌ v changes
    QFile file("doc.txt");
    if (!file.open(QIODevice::WriteOnly)) {
        warning(u8"S˙bor sa nepodarilo otvoriù.");
    }
    else {
        int i;
        QTextStream out(&file);
        for (i = 0; i < loginy.size(); i++) {
            if (loginy[i]->gettyp() == 0)
                out << loginy[i]->gettyp() << "," << loginy[i]->getlogin() << "," << loginy[i]->getheslo() << ",\n";
            else if (loginy[i]->gettyp() == 1)
                out << loginy[i]->gettyp() << "," << loginy[i]->getlogin() << "," << loginy[i]->getheslo() << "," << loginy[i]->getpoz() << ",\n";
            else if (loginy[i]->gettyp() == 2 || loginy[i]->gettyp() == 3)
                out << loginy[i]->gettyp() << "," << loginy[i]->getlogin() << "," << loginy[i]->getheslo() << "," << loginy[i]->getmeno() << "," << loginy[i]->getpriezvisko() << "," << loginy[i]->getadresa() << "," << loginy[i]->getminute() << "\n";
        }
    }
    file.close();
}

void lekaren::zapisLieky() {     //zapise lieky nanovo do suboru s upravenym skladom po nakupe alebo cely upraveny po zasahu admina, zavola sa pri odhlaseni
    QFile zapis("lieky.txt");
    if (!zapis.open(QIODevice::WriteOnly))
        warning(u8"S˙bor sa nepodarilo otvoriù.");
    else {
        int i;
        QTextStream out(&zapis);
        for (i = 0; i < lieky.size(); i++) {
            out << lieky[i]->getnazov() << "," << lieky[i]->getcena() << "," << sklad[i] << "\n";
        }
    }
    zapis.close();
}

void lekaren::on_actionFile_triggered() {      //malo by umoznit nahrat novy txt s liekmi (one day)
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", tr("Text Files (*.txt; *.csv)"));
    QFile file(fileName);
    int i,j, k=lieky.size();
    bool nasiel = false;
    if (!file.open(QIODevice::ReadOnly))
        warning(u8"S˙bor sa nepodarilo otvoriù.");
    else {
        QVector<liek*> subor;
        QVector<liek*> temp;
        QVector<int> suborsklad;
        QVector<int> temps;
        
        while (!file.atEnd()) {                     //vytvori alternativny sklad zo suboru
            QString line = file.readLine();
            QString name = (line.split(',').at(0));
            float cena = (line.split(',').at(1).toFloat());
            int ks = (line.split(',').at(2).toInt());
            subor.push_back(new liek(name, cena));
            suborsklad.append(ks);
        }
        file.close();
        for (j = 0; j < lieky.size(); j++) {            // vynuluje sklad pre pripad, ze lieky ktore boli na sklade boli vyradene z predaja
            sklad[j] = 0;
        }
        for (i = 0; i < subor.size(); i++) {            //porovna povodny sklad so skladom zo suboru
            for (j = 0; j < k; j++) {        
                if (subor[i]->getnazov() == lieky[j]->getnazov()) {     //ak najde zhodne lieky prepise cenu v povodnom sklade a pocet zasob
                    lieky[j]->setcena(subor[i]->getcena());
                    sklad[j] = suborsklad[i];
                    nasiel = true;
                }
                if (!nasiel && j == lieky.size() - 1) {     //ak liek doteraz nebol na sklade, prida ho na koniec
                    QString nazov;  
                    float cena;
                    temps.append(suborsklad[i]);
                    nazov = subor[i]->getnazov();
                    cena = subor[i]->getcena();
                    temp.push_back(new liek(nazov, cena));                                         //vytvorit este jeden temp toto nefici
                }
            }
            nasiel = false;
        }
        for (i = 0; i < temp.size(); i++) {
            sklad.append(temps[i]);
            lieky.push_back(temp[i]);
        }
        subor.clear();
        suborsklad.clear();
        zapisLieky();
        fill_table_admin();
    }
}

void lekaren::on_rem3_clicked() {
    int i,index,j;
    index = ui.UsersComboBox->currentIndex();
    //qDebug() << index;
    if (loginy[index]->getlogin() == prihlaseny->getlogin()) {
        warning(u8"Nie je moûnÈ odstr·niù pouûÌvateæa");
    }
    else {
        for (i = 0; i < objednavky.size(); i++) {
            if (objednavky[i]->getlogin() == loginy[index]->getlogin()) {
                for (j = 0; j < objednavky[i]->getkos().size(); j++) {
                    sklad[objednavky[i]->getindex(j)]++;
                }
                objednavky.remove(i);
            }
        }
        loginy.remove(index);
        zapisUsers();
        ReloadComboUsers();
        ReloadComboBox();
    }
}

void lekaren::on_User_triggered() {           //malo by otvorit dialog na zapis noveho uzivatela (one day)
    change = new Dialog(this);
    connect(change, SIGNAL(accepted()), this, SLOT(changeAccepted()));
    change->exec();

}

void lekaren::changeAccepted() {
    int i;
    bool obsadene=false;
    Dialog* change = static_cast<Dialog *>(sender());
    int typ = change->getTyp();
    QString login = change->getLogin();
    for (i = 0; i < loginy.size(); i++) {
        if (login == loginy[i]->getlogin()) {
            warning(u8"PouûÌvateæ s t˝mto menom uû existuje.");
            obsadene=true;
        }
    } 
    QString heslo = encrypt(change->getPswd());
    QString pozicia = change->getPosition();
    QString meno = change->getName();
    QString priezvisko = change->getSurname();
    QString adresa = change->getAdress();
    float minute = change->getMinute().toFloat();
    if (!obsadene) {
        if (typ == 0)
            loginy.push_back(new admin(typ, login, heslo));
        else if (typ == 1)
            loginy.push_back(new staff(typ, login, heslo, pozicia));
        else if (typ == 2)
            loginy.push_back(new zakaznik(typ, login, heslo, meno, priezvisko, adresa, minute));
        else if (typ == 3)
            loginy.push_back(new premium(typ, login, heslo, meno, priezvisko, adresa, minute));
        zapisUsers();
        ReloadComboUsers();
    }
}

void lekaren::on_actionTxt_triggered() {    //vytvori txt s aktualnymi objednavkami
    QFile zapis("txtStaff.txt");
    if (!zapis.open(QIODevice::WriteOnly))
        warning(u8"S˙bor sa nepodarilo otvoriù.");
    else {
        int i,j;
        QTextStream out(&zapis);
        for (i = 0; i < objednavky.size(); i++) {
            out << "Objednavka pouzivatela: " << objednavky[i]->getlogin() << "\n";
            for (j = 0; j < objednavky[i]->getkos().size(); j++)
                out << lieky[objednavky[i]->getindex(j)]->getnazov() << "       " << lieky[i]->getcena() << "\n";
            out << "**************************\n";
        }
    }
    zapis.close();
}

void lekaren::zapisObjednavky() {
    QFile zapis("objednavky.txt");
    if (!zapis.open(QIODevice::WriteOnly))
        warning(u8"S˙bor sa nepodarilo otvoriù.");
    else {
        int i, j;
        QTextStream out(&zapis);
        for (i = 0; i < objednavky.size(); i++) {
            out << objednavky[i]->getlogin() << "," << objednavky[i]->getsum() << ",";
            for (j = 0; j < objednavky[i]->getkos().size(); j++) {
                out << lieky[objednavky[i]->getindex(j)]->getnazov() << ",";
            }
            out << "\n";
        }
    }
    zapis.close();
}

void lekaren::objednavkyload() {
    objednavky.clear();
    QFile file("objednavky.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        warning(u8"Nepodarilo sa otvoriù s˙bor s objedn·vkami.");
    }
    QVector<int> kos;
    int i = 2,j;
    const QChar newline(QChar('\n'));
    QString liek;
    while (!file.atEnd()) {
        QString line = file.readLine();
        QString login = (line.split(',').at(0));
        float cena = (line.split(',').at(1).toFloat());
        while (line.split(',').at(i) != newline) {
            liek = (line.split(',').at(i));
            for (j = 0; j < lieky.size();j++) {
                if (lieky[j]->getnazov() == liek){
                    kos.append(j);
                    i++;
                }
            }           
        }
        i = 2;
        objednavky.push_back(new objednavka(cena, login, kos));
        kos.clear();
    }
}