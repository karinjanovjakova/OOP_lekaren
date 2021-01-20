#pragma once

#include <QtWidgets>
#include "ui_lekaren.h"
#include "changes.h"

class objednavka
{
private:
    float sum;
    QString login;
    QVector<int> kos;
public:
    objednavka() {
        sum = 0;
        login = "";
        kos = QVector<int>(0);
    };
    objednavka(float a, QString b, QVector<int> c) {
        sum = a;
        login = b;
        kos = c;
    };
    ~objednavka() {};

    void setsum(float a) { sum = a; };
    float getsum() { return sum; };
    QString getlogin() { return login; };
    void setlogin(QString a) { login = a; };
    QVector <int> getkos() { return kos; };
    void setkos(QVector<int> a) { kos = a; };
    int getindex(int i) { return kos[i]; };

};

class admin
{
protected:
    int typ = -1;
    QString login = "";
    QString heslo = "";
public:
    admin() {
        typ = -1;
        login = "";
        heslo = "";

    };
    admin(int a, QString log, QString psw) {
        typ = a;
        login = log;
        heslo = psw;
    };
    ~admin() {};

    QString getlogin() { return login; };
    void setlogin(QString a) { login = a; };
    QString getheslo() { return heslo; };
    void setheslo(QString a) { heslo = a; };
    int gettyp() { return typ; };
    void settyp(int a) { typ = a; };

    //virtual staff
    virtual QString getpoz() { return ""; };
    virtual void setpoz(QString a) {};
    //virtual customer
    virtual QString getmeno() { return ""; };
    virtual void setmeno(QString a) {};
    virtual QString getpriezvisko() { return ""; };
    virtual void setpriezvisko(QString a) {};
    virtual QString getadresa() { return ""; };
    virtual void setadresa(QString a) {};
    virtual float getminute() { return 0; };
    virtual void setminute(float a) {};
    virtual QVector<int> getkos() { return QVector<int>(0); };
    virtual void dokosa(int index) {};
    virtual void odober(int index) {};
    virtual int sizekos() { return 0; };
    virtual int getindex(int i) { return 0; };
    virtual void setindex(int index, int i) {};
    virtual void append(int index) {};
    virtual float getsuma() { return 0; };
    virtual void sumaplus(float a) {};
    virtual void sumaminus(float a) {};
    virtual void setsuma(float a) {};
    virtual void clearkos() {};
    //virtual premium
    virtual int getzlava() { return 0; };
    virtual void setzlava() {};
};

class liek
{
private:
    QString nazov;
    float cena; 
public:
    liek() {
        nazov = "";
        cena = 0;
    };
    liek(QString a, float b) {
        nazov = a;
        cena = b;
    };
    ~liek() {};

    void setnazov(QString a) { nazov = a; };
    QString getnazov() { return nazov; };
    void setcena(float a) { cena = a; };
    float getcena() { return cena; };
};

class lekaren : public QMainWindow
{
    Q_OBJECT

public:
    lekaren(QWidget* parent = Q_NULLPTR);

    QString encrypt(QString heslo);
    QVector<admin*> loginy;
    admin* prihlaseny;
    void pouzivatelia();
    void zapisUsers();
    void start();
    QVector<liek*> lieky;
    QVector<int> sklad;
    void liekyload ();
    void fill_table_admin();
    void fill_table_zak();
    QVector <objednavka*> objednavky;
    void ReloadComboBox();
    void ReloadComboUsers();
    void zapisLieky();


private:
    Ui::lekarenClass ui;
    //changes* change = nullptr;

private slots:
    void on_login_clicked();
    void on_logout_clicked();

    void on_Add_clicked();

    void on_kos_clicked();
    void on_rem_clicked();
    void on_pay_clicked();

    void on_rem2_clicked();

    void on_actionFile_triggered();
    //void on_Users_triggered();
    //void on_UserAdd_triggered();

    //void on_actionObj_triggered();

    void on_reload_clicked();
    void on_checkBox_clicked();
    void on_Save_clicked();
};

class staff : public admin
{
private:
    QString poz;
public:
    staff() : admin () {
        poz = "";
    };
    staff(int a, QString log, QString psw, QString pozicia) : admin (a, log, psw) {
        poz = pozicia;
    };
    ~staff() {};

    QString getpoz() { return poz; };
    void setpoz(QString a) { poz = a; };

};

class zakaznik : public admin
{
protected:
    QString meno = "";
    QString priezvisko = "";
    QString adresa = "";
    float minute = 0;
    float suma=0;
    QVector<int> kosik;

public:
    zakaznik() : admin() {
        meno = "";
        priezvisko = "";
        adresa = "";
        minute = 0;
        suma = 0;
    };
    zakaznik(int a, QString usr, QString hes, QString men, QString priez, QString adr, float b) : admin(a, usr, hes) {
        meno = men;
        priezvisko = priez;
        adresa = adr;
        minute = b;
        suma = 0;
    };
    ~zakaznik() {};

    QString getmeno() { return meno; };
    void setmeno(QString a) { meno = a; };
    QString getpriezvisko() { return priezvisko; };
    void setpriezvisko(QString a) { priezvisko = a; };
    QString getadresa() { return adresa; };
    void setadresa(QString a) { adresa = a; };
    float getminute() { return minute; };
    void setminute(float a) { minute = a; };
    float getsuma() { return suma; };
    void setsuma(float a) { suma = a; };
    void sumaplus(float a) { suma += a; };
    void sumaminus(float a) { suma -= a; };

    QVector<int> getkos() { return kosik; };
    void dokosa(int index) { kosik[index]++; };
    void odober(int index) { kosik.remove(index); };
    int sizekos() { return kosik.size(); };
    int getindex(int i) { return kosik[i]; };
    void setindex(int index, int i) { kosik[index] = i; };
    void append(int index) { kosik.push_back(index); };
    void clearkos() { kosik.clear(); };
};

class premium : public zakaznik
{
private:
    int zlava;
public:
    premium() : zakaznik() {
        zlava = 0;
    };
    premium(int a, QString usr, QString hes, QString men, QString priez, QString adr, float b) : zakaznik (a, usr, hes, men, priez, adr, b) {
        //zlava = 0;
        setzlava();
    };
    ~premium() {};

    void setzlava() {
        zlava = minute / 100;
        if (zlava >= 20)
            zlava = 20;
    };
    int getzlava() { return zlava; };

};

