#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QErrorMessage>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    count();
    connect(ui->alphabet, SIGNAL(textChanged(QString)), this, SLOT(count()));

    check();
    //
    connect(ui->aff_alf_zas, SIGNAL(valueChanged(QString)), this, SLOT(check()));
    connect(ui->aff_alf_ras, SIGNAL(valueChanged(QString)), this, SLOT(check()));

    //проверка правильности введенных данных для рекуррентного аффинного шифра
    connect(ui->aff_rec_alf_zas1, SIGNAL(valueChanged(QString)), this, SLOT(check()));
    connect(ui->aff_rec_alf_zas2, SIGNAL(valueChanged(QString)), this, SLOT(check()));
    connect(ui->aff_rec_alf_ras1, SIGNAL(valueChanged(QString)), this, SLOT(check()));
    connect(ui->aff_rec_alf_ras2, SIGNAL(valueChanged(QString)), this, SLOT(check()));

    //шифр Хилла
    hill_translate_in();
    hill_translate_ou();
    connect(ui->hill_key_zas, SIGNAL(textChanged(QString)), this, SLOT(hill_translate_in()));
    connect(ui->hill_key_ras, SIGNAL(textChanged(QString)), this, SLOT(hill_translate_ou()));

    //рекуррентный шифр Хилла
    rec_hill_translate_in();
    rec_hill_translate_ou();
    connect(ui->rec_hill_key_zas, SIGNAL(textChanged(QString)), this, SLOT(rec_hill_translate_in()));
    connect(ui->rec_hill_key_zas_2, SIGNAL(textChanged(QString)), this, SLOT(rec_hill_translate_in()));
    connect(ui->rec_hill_key_ras, SIGNAL(textChanged(QString)), this, SLOT(rec_hill_translate_ou()));
    connect(ui->rec_hill_key_ras_2, SIGNAL(textChanged(QString)), this, SLOT(rec_hill_translate_ou()));

    //шифр Виженера
    check_vizhener();
    connect(ui->vizhener_zas, SIGNAL(textChanged(QString)),this, SLOT(check_vizhener()));
    connect(ui->vizhener_ras, SIGNAL(textChanged(QString)),this, SLOT(check_vizhener()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//======================дополнительные=функции======================================
void MainWindow::count(){
    int alphabet_size = ui->alphabet->text().size();
    (*alphabet) = ui->alphabet->text();
    ui->lcdNumber->display(alphabet_size);
    check();

    ui->aff_alf_ras->setMaximum(alphabet_size - 1);
    ui->aff_alf_zas->setMaximum(alphabet_size - 1);
    ui->aff_bet_ras->setMaximum(alphabet_size - 1);
    ui->aff_bet_zas->setMaximum(alphabet_size - 1);

    ui->aff_rec_alf_zas1->setMaximum(alphabet_size - 1);
    ui->aff_rec_alf_zas2->setMaximum(alphabet_size - 1);
    ui->aff_rec_bet_zas1->setMaximum(alphabet_size - 1);
    ui->aff_rec_bet_zas2->setMaximum(alphabet_size - 1);
    ui->aff_rec_alf_ras1->setMaximum(alphabet_size - 1);
    ui->aff_rec_alf_ras2->setMaximum(alphabet_size - 1);
    ui->aff_rec_bet_ras1->setMaximum(alphabet_size - 1);
    ui->aff_rec_bet_ras2->setMaximum(alphabet_size - 1);
}

int MainWindow::search(QString a){
    int j = 0;
    while((*alphabet).at(j) != a.at(0)){
        if (j+1 < ((*alphabet).size()))
            j++;
        else
            return -1;
    }
    return j;
}

int MainWindow::Evklid(int a, int b, int c){
    int otvet[4];
    int bb = b, x_2 = 1, x_1 = 0, y_2 = 0, y_1 = 1, x = 0, y = 0, q = 0, r = 0;
    while (b>0){
        q=a/b;
        r=a%b;
        x=x_2-q*x_1;
        y=y_2-q*y_1;

        a=b;
        b=r;
        x_2=x_1;
        x_1=x;
        y_2=y_1;
        y_1=y;
    }

    otvet[0]=(x%bb);                            //остаток от деления
    otvet[1]=x_2;                               //обратное число
    otvet[2]=y_2;                               //
    otvet[3]=a;                                 //НОД
    if(c == 0)
        return otvet[0];
    else {
        if (c == 1)
            return otvet[1];
        else {
            if (c == 2)
                return otvet[2];
            else
                return otvet[3];
        }
    }
}

void MainWindow::check(){
    if(ui->tabWidget->currentIndex() == 0){
        if(Evklid(ui->aff_alf_zas->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            ui->aff_zashif->setEnabled(false);
        else
            ui->aff_zashif->setEnabled(true);

        if(Evklid(ui->aff_alf_ras->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            ui->aff_rashif->setEnabled(false);
        else
            ui->aff_rashif->setEnabled(true);
    }
    if(ui->tabWidget->currentIndex() == 1){
        int oshibka_ras = 0, oshibka_zas = 0;

        if(Evklid(ui->aff_rec_alf_zas1->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            oshibka_zas++;
        if(Evklid(ui->aff_rec_alf_zas2->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            oshibka_zas++;

        if(Evklid(ui->aff_rec_alf_ras1->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            oshibka_ras++;
        if(Evklid(ui->aff_rec_alf_ras1->text().toInt(), ui->alphabet->text().size(), 3) != 1)
            oshibka_ras++;

        if (oshibka_zas==0){
            ui->aff_rec_zashif->setEnabled(true);
        }
        else ui->aff_rec_zashif->setEnabled(false);

        if (oshibka_ras==0){
            ui->aff_rec_rashif->setEnabled(true);
        }
        else ui->aff_rec_rashif->setEnabled(false);
    }
}

void MainWindow::on_pushButton_clicked()
{
    //если выбран аффинный шифр
    if(ui->tabWidget->currentIndex() == 0){
        if(Evklid(ui->aff_alf_zas->text().toInt(), ui->alphabet->text().size(), 3) != 1){
            if(Evklid(ui->aff_alf_ras->text().toInt(), ui->alphabet->text().size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("ПЕРВЫЕ значения ключей для зашифрования и расшифрования не являются взаимнопростыми с размером алфавита.\nВведите другие значения."),
                                   nullptr, this);
                msgBox.exec();
            }
            else {
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("ПЕРВОЕ значение ключа для зашифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
        else {
            if(Evklid(ui->aff_alf_ras->text().toInt(), ui->alphabet->text().size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("ПЕРВОЕ значение ключа для расшифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else {
                QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                                   QString::fromUtf8("Все условия соблюдены.\nВведенные ключи можно использовать."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
    }
    //если выбран аффинный рекуррентный шифр
    if(ui->tabWidget->currentIndex() == 1)
    {
        //проверка введенных значений альфа для зашифрования
        int oshibka = 0;
        if(Evklid(ui->aff_rec_alf_zas1->text().toInt(), ui->alphabet->text().size(), 3) != 1){
            QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                               QString::fromUtf8("ПЕРВОЕ значение ключа для зашифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                               nullptr, this);
            msgBox.exec();
            oshibka++;
        }
        if(Evklid(ui->aff_rec_alf_zas2->text().toInt(), ui->alphabet->text().size(), 3) != 1){
            QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                               QString::fromUtf8("ТРЕТЬЕ значение ключа для зашифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                               nullptr, this);
            msgBox.exec();
            oshibka++;
        }

        //проверка введенных значений альфа для расшифрования
        if(Evklid(ui->aff_rec_alf_ras1->text().toInt(), ui->alphabet->text().size(), 3) != 1){
            QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                               QString::fromUtf8("ПЕРВОЕ значение ключа для расшифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                               nullptr, this);
            msgBox.exec();
            oshibka++;
        }
        if(Evklid(ui->aff_rec_alf_ras2->text().toInt(), ui->alphabet->text().size(), 3) != 1){
            QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                               QString::fromUtf8("ТРЕТЬЕ значение ключа для расшифрования не является взаимнопростым с размером алфавита.\nВведите другое значение."),
                               nullptr, this);
            msgBox.exec();
            oshibka++;
        }

        if (oshibka == 0){
            QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                               QString::fromUtf8("Все условия соблюдены.\nВведенные ключи можно использовать."),
                               nullptr, this);
            msgBox.exec();
        }
    }
    //если выбран шифр Хилла
    if(ui->tabWidget->currentIndex() == 2){
        QString temp1 = ui->hill_key_zas->text();
        QString temp2 = ui->hill_key_ras->text();
        while(temp1.size()!=9)
            temp1 += " ";
        while(temp2.size()!=9)
            temp2 += " ";
        int tmp1[3][3],tmp2[3][3];
        for(int i=0;i<9;i++){
            tmp1[i/3][i%3] = search(temp1.at(i));
            tmp2[i/3][i%3] = search(temp2.at(i));
        }

        if((Evklid(deter3(tmp1), (*alphabet).size(), 3) != 1 ) && (Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1)){
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя матрицы зашифрования и размера алфавита не равен 1.\nНОД определителя матрицы расшифрования и размера алфавита не равен 1.\nВведите другие значения."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя матрицы зашифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
        else{
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя матрицы расшифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                                   QString::fromUtf8("Все условия соблюдены.\nВведенные ключи можно использовать."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
    }
    //если выбран рекуррентный шифр Хилла
    if(ui->tabWidget->currentIndex() == 3){
        QString temp1 = ui->rec_hill_key_zas->text();
        QString temp2 = ui->rec_hill_key_zas_2->text();
        while(temp1.size()!=9)
            temp1 += " ";
        while(temp2.size()!=9)
            temp2 += " ";
        int tmp1[3][3],tmp2[3][3];
        for(int i=0;i<9;i++){
            tmp1[i/3][i%3] = search(temp1.at(i));
            tmp2[i/3][i%3] = search(temp2.at(i));
        }

        if((Evklid(deter3(tmp1), (*alphabet).size(), 3) != 1 ) && (Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1)){
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя первой матрицы для зашифрования и размера алфавита не равен 1.\nНОД определителя второй матрицы для зашифрования и размера алфавита не равен 1.\nВведите другие значения."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя первой матрицы для зашифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
        else{
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя второй матрицы для зашифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                                   QString::fromUtf8("Все условия для зашифрования соблюдены.\nВведенные ключи можно использовать."),
                                   nullptr, this);
                msgBox.exec();
            }
        }

        temp1 = ui->rec_hill_key_zas->text();
        temp2 = ui->rec_hill_key_zas_2->text();
        while(temp1.size()!=9)
            temp1 += " ";
        while(temp2.size()!=9)
            temp2 += " ";
        for(int i=0;i<9;i++){
            tmp1[i/3][i%3] = search(temp1.at(i));
            tmp2[i/3][i%3] = search(temp2.at(i));
        }

        if((Evklid(deter3(tmp1), (*alphabet).size(), 3) != 1 ) && (Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1)){
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя первой матрицы для расшифрования и размера алфавита не равен 1.\nНОД определителя второй матрицы для расшифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя первой матрицы для расшифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
        else{
            if(Evklid(deter3(tmp2), (*alphabet).size(), 3) != 1){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("НОД определителя второй матрицы для расшифрования и размера алфавита не равен 1.\nВведите другое значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                                   QString::fromUtf8("Все условия соблюдены.\nВведенные ключи можно использовать."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
    }
    //если выбран шифр Виженера
    if(ui->tabWidget->currentIndex() == 4){
        if(ui->vizhener_zas->text() == ""){
            if(ui->vizhener_ras->text() == ""){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("Поля для ключей шифрования и расшифрования пустые.\nВведите значения."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("Поле для ключа шифрования пустое.\nВведите значение."),
                                   nullptr, this);
                msgBox.exec();
            }
        }
        else{
            if(ui->vizhener_ras->text() == ""){
                QMessageBox msgBox(QMessageBox::Warning, QString::fromUtf8("Предупреждение"),
                                   QString::fromUtf8("Поле для ключа расшифрования пустое.\nВведите значение."),
                                   nullptr, this);
                msgBox.exec();
            }
            else{
                QMessageBox msgBox(QMessageBox::Information, QString::fromUtf8("Сообщение"),
                                   QString::fromUtf8("Все условия соблюдены.\nВведенные ключи можно использовать."),
                                   nullptr, this);
                msgBox.exec();
            }
        }

    }
}

int MainWindow::deter2(int a[2][2]){
    return (a[0][0]*a[1][1]-a[0][1]*a[1][0]);
}

int MainWindow::deter3(int a[3][3]){
    return (a[0][0]*a[1][1]*a[2][2] + a[1][0]*a[2][1]*a[0][2] + a[0][1]*a[1][2]*a[2][0]
            - a[2][0]*a[1][1]*a[0][2] - a[0][0]*a[2][1]*a[1][2] - a[1][0]*a[0][1]*a[2][2]);
}

//======================дополнительные=функции======================================

//======================действия=с=аффинным=шифром==================================
void MainWindow::on_aff_zashif_clicked(){
    on_aff_cl_ou_clicked();
    //объявление полезных переменных и получение необходимых данных
    int alfa = ui->aff_alf_zas->text().toInt();
    int beta = ui->aff_bet_zas->text().toInt();
    int j;                                      //переменная - положение искомой буквы в алфавите
    QString input = ui->aff_in->text();
    QString out;
    for(int i = 0; i < input.size(); i++){
        j = search(input.at(i));                //ищем положение i-го символа в нашем алфавите
        if(j == -1)
            out += input.at(i);
        else {
            //дописываем в переменную out символ с расчитанным положением в алфавите
            out += (*alphabet).at( (alfa * j + beta) % ((*alphabet).size()) );
        }
    }
    ui->aff_ou->setText(out);
}

void MainWindow::on_aff_cl_in_clicked()
{
    ui->aff_in->setText("");
}

void MainWindow::on_aff_cl_ou_clicked()
{
    ui->aff_ou->setText("");
}

void MainWindow::on_aff_rashif_clicked(){
    on_aff_cl_in_clicked();
    //объявление полезных переменных и получение необходимых данных
    int alfa_1 = Evklid( ui->aff_alf_ras->text().toInt(), (*alphabet).size(), 1);
    int beta = ui->aff_bet_ras->text().toInt();
    int j;                                      //переменная - положение искомой буквы в алфавите
    QString input = ui->aff_ou->text();
    QString out;
    for(int i = 0; i < input.size(); i++){
        j = search(input.at(i));                //ищем положение i-го символа в нашем алфавите
        if(j == -1)
            out += input.at(i);
        else {
            int p = (alfa_1 * (j - beta)) % ((*alphabet).size());
            if(p < 0 ){
                p += (*alphabet).size();
            }
            //дописываем в переменную out символ с расчитанным положением в алфавите
            out += (*alphabet).at( p );
        }
    }
    ui->aff_in->setText(out);
}
//======================действия=с=аффинным=шифром==================================

//===================действия=с=рекуррентным=аффинным=шифром========================
void MainWindow::on_aff_rec_zashif_clicked()
{
    on_aff_rec_cl_ou_clicked();
    //объявление полезных переменных и получение необходимых данных
    int alfa[3], beta[3];
    alfa[0] = ui->aff_rec_alf_zas1->text().toInt();
    alfa[1] = ui->aff_rec_alf_zas2->text().toInt();
    beta[0] = ui->aff_rec_bet_zas1->text().toInt();
    beta[1] = ui->aff_rec_bet_zas2->text().toInt();
    int j;
    QString input = ui->aff_rec_in->text();
    QString out;
    for(int i = 0; i < input.size(); i++){
        //расчет ключей
        if(i > 1){
            alfa[2] = (alfa[0] * alfa[1])%((*alphabet).size());
            beta[2] = (beta[0] + beta[1])%((*alphabet).size());
        }
        else {
            if(i == 0){
                alfa[2] = alfa[0];
                beta[2] = beta[0];
            }
            else{
                alfa[2] = alfa[1];
                beta[2] = beta[1];
            }
        }

        alfa[0] = alfa[1];
        alfa[1] = alfa[2];
        beta[0] = beta[1];
        beta[1] = beta[2];
        //расчет ключей закончен

        j = search(input.at(i));                //ищем положение i-го символа в нашем алфавите
        if(j == -1)
            out += input.at(i);
        else {
            //дописываем в переменную out символ с расчитанным положением в алфавите
            out += (*alphabet).at( (alfa[2] * j + beta[2]) % ((*alphabet).size()) );
        }
    }
    ui->aff_rec_ou->setText(out);
}

void MainWindow::on_aff_rec_cl_in_clicked()
{
    ui->aff_rec_in->setText("");
}

void MainWindow::on_aff_rec_cl_ou_clicked()
{
    ui->aff_rec_ou->setText("");
}

void MainWindow::on_aff_rec_rashif_clicked()
{
    on_aff_rec_cl_in_clicked();
    //объявление полезных переменных и получение необходимых данных
    int alfa[3], beta[3];
    alfa[0] = ui->aff_rec_alf_ras1->text().toInt();
    alfa[1] = ui->aff_rec_alf_ras2->text().toInt();
    beta[0] = ui->aff_rec_bet_ras1->text().toInt();
    beta[1] = ui->aff_rec_bet_ras2->text().toInt();
    int j;
    QString input = ui->aff_rec_ou->text();
    QString out;
    for(int i = 0; i < input.size(); i++){
        //расчет ключей
        if(i > 1){
            alfa[2] = (alfa[0] * alfa[1])%((*alphabet).size());
            beta[2] = (beta[0] + beta[1])%((*alphabet).size());
        }
        else {
            if(i == 0){
                alfa[2] = alfa[0];
                beta[2] = beta[0];
            }
            else{
                alfa[2] = alfa[1];
                beta[2] = beta[1];
            }
        }

        alfa[0] = alfa[1];
        alfa[1] = alfa[2];
        beta[0] = beta[1];
        beta[1] = beta[2];

        j = search(input.at(i));                //ищем положение i-го символа в нашем алфавите
        if(j == -1)
            out += input.at(i);
        else {
            int p = (Evklid(alfa[2],(*alphabet).size(), 1) * (j - beta[2])) % ((*alphabet).size());
            if(p < 0 ){
                p += (*alphabet).size();
            }
            //дописываем в переменную out символ с расчитанным положением в алфавите
            out += (*alphabet).at( p );
        }
    }
    ui->aff_rec_in->setText(out);
}
//===================действия=с=рекуррентным=аффинным=шифром========================


//===================действия=с=шифром=Хилла========================================
void MainWindow::on_hill_cl_in_clicked()
{
    ui->hill_in->setText("");
}

void MainWindow::on_hill_cl_ou_clicked()
{
    ui->hill_ou->setText("");
}

void MainWindow::hill_translate_in(){
    QStandardItemModel *model = new QStandardItemModel;
    QString temp = ui->hill_key_zas->text();
    while(temp.size()!=9)
        temp += " ";
    int tmp[3][3];
    for(int i=0;i<9;i++){
        QStandardItem *item;
        int znac = search(temp.at(i));
        tmp[i/3][i%3] = znac;
        item = new QStandardItem(QString::number(znac,10));
        model->setItem(i/3, i%3, item);
    }
    ui->hill_table_in->setModel(model);
    ui->hill_table_in->resizeRowsToContents();
    ui->hill_table_in->resizeColumnsToContents();
    if(Evklid(deter3(tmp), (*alphabet).size(), 3) == 1)
        ui->hill_zashif->setEnabled(true);
    else
        ui->hill_zashif->setEnabled(false);
}

void MainWindow::hill_translate_ou(){
    QStandardItemModel *model = new QStandardItemModel;
    QString temp = ui->hill_key_ras->text();
    while(temp.size()!=9)
        temp += " ";
    int tmp[3][3];
    for(int i=0;i<9;i++){
        QStandardItem *item;
        int znac = search(temp.at(i));
        tmp[i/3][i%3] = znac;
        item = new QStandardItem(QString::number(znac,10));
        model->setItem(i/3, i%3, item);
    }
    ui->hill_table_ou->setModel(model);
    ui->hill_table_ou->resizeRowsToContents();
    ui->hill_table_ou->resizeColumnsToContents();
    qDebug()<<"определитель: "<<deter3(tmp);
    if(Evklid(deter3(tmp), (*alphabet).size(), 3) == 1)
        ui->hill_rashif->setEnabled(true);
    else
        ui->hill_rashif->setEnabled(false);
}

void MainWindow::multiplication(){
    //tab_key - таблица с позициями символов из ключа
    int tab_key[3][3];
    for(int i=0;i<9;i++)
        tab_key[i/3][i%3] = search((*key).at(i));
    //tab_phrase - таблица с позициями символов из фразы
    int tab_phrase[((*phrase).size()/3)][3];
    for(int i=0;i<(*phrase).size();i++)
        tab_phrase[i/3][i%3] = search((*phrase).at(i));
    int alf_size = (*alphabet).size();
    QString shifr_text;
    for(int k=0;k<((*phrase).size()/3);k++){
        for(int i=0;i<3;i++){
            int rez=0;
            for(int j=0;j<3;j++){
                rez += tab_key[i][j]*tab_phrase[k][j];
            }
            shifr_text += (*alphabet).at(rez % alf_size);
        }
    }
    *shifr = shifr_text;
}

void MainWindow::remultiplication(){
    //tab_key - таблица с позициями символов из ключа
    int tab_key[3][3];
    for(int i=0;i<9;i++)
        tab_key[i/3][i%3] = search((*key).at(i));
    //tab_phrase - таблица с позициями символов из фразы
    int tab_phrase [((*phrase).size()/3)][3];
    for(int i=0;i<(*phrase).size();i++)
        tab_phrase[i/3][i%3] = search((*phrase).at(i));
    int opred = deter3(tab_key);
    int x = Evklid(opred, (*alphabet).size(), 1);
    if(opred>0){
        if(x<0)
            x = x + (*alphabet).size();
    }
    else{
        if(x<0)
            x = abs(x);
    }
    int reverse_key[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            //получение элемента для матрицы алгебраических дополнений
            int tmp[2][2], stroka=0, stolbec=0;
            for(int ii=0;ii<3;ii++){
                if(ii != i){
                    for(int jj=0;jj<3;jj++){
                        if(jj != j){
                            tmp[stroka][stolbec] = tab_key[ii][jj];
                            stolbec ++;
                        }
                    }
                    stroka ++;
                    stolbec = 0;
                }
            }
            //
            reverse_key[j][i] = int(pow((-1),(i+j)))*(deter2(tmp));
            reverse_key[j][i] = (reverse_key[j][i]*x)%((*alphabet).size());
            while(reverse_key[j][i]<0)
                reverse_key[j][i] += (*alphabet).size();
        }
    }
    int alf_size = (*alphabet).size();
    QString shifr_text;
    for(int k=0;k<((*phrase).size()/3);k++){
        for(int i=0;i<3;i++){
            int rez=0;
            for(int j=0;j<3;j++)
                rez += reverse_key[i][j]*tab_phrase[k][j];
            shifr_text += (*alphabet).at(rez % alf_size);
        }
    }
    *shifr = shifr_text;
}

void MainWindow::on_hill_zashif_clicked()
{
    ui->hill_ou->setText("");
    QString tmp = ui->hill_key_zas->text();
    while(tmp.size()!=9)
        tmp += " ";
    *key = tmp;
    tmp = ui->hill_in->text();
    while(tmp.size()%3 != 0)
        tmp += " ";
    *phrase = tmp;
    multiplication();
    ui->hill_ou->setText(*shifr);
}

void MainWindow::on_hill_rashif_clicked()
{
    ui->hill_in->setText("");
    QString tmp = ui->hill_key_ras->text();
    while(tmp.size()!=9)
        tmp += " ";
    *key = tmp;
    tmp = ui->hill_ou->text();
    while(tmp.size()%3 != 0)
        tmp += " ";
    *phrase = tmp;
    remultiplication();
    ui->hill_in->setText(*shifr);
}
//===================действия=с=шифром=Хилла========================================

//===================действия=с=рекуррентным=шифром=Хилла===========================
void MainWindow::rec_hill_translate_in(){
    QStandardItemModel *model1 = new QStandardItemModel;
    QStandardItemModel *model2 = new QStandardItemModel;
    QString temp1 = ui->rec_hill_key_zas->text();
    QString temp2 = ui->rec_hill_key_zas_2->text();
    while(temp1.size()!=9)
        temp1 += " ";
    while(temp2.size()!=9)
        temp2 += " ";
    int tmp1[3][3],tmp2[3][3];
    for(int i=0;i<9;i++){
        QStandardItem *item;
        int znac1 = search(temp1.at(i));
        int znac2 = search(temp2.at(i));
        tmp1[i/3][i%3] = znac1;
        tmp2[i/3][i%3] = znac2;
        item = new QStandardItem(QString::number(znac1,10));
        model1->setItem(i/3, i%3, item);
        item = new QStandardItem(QString::number(znac2,10));
        model2->setItem(i/3, i%3, item);
    }
    ui->rec_hill_table_in->setModel(model1);
    ui->rec_hill_table_in->resizeRowsToContents();
    ui->rec_hill_table_in->resizeColumnsToContents();
    ui->rec_hill_table_in_2->setModel(model2);
    ui->rec_hill_table_in_2->resizeRowsToContents();
    ui->rec_hill_table_in_2->resizeColumnsToContents();

    qDebug()<<"1 ключ "<<(Evklid(deter3(tmp1), (*alphabet).size(), 3));
    qDebug()<<"2 ключ "<<(Evklid(deter3(tmp2), (*alphabet).size(), 3));
    if((Evklid(deter3(tmp1), (*alphabet).size(), 3) == 1 ) && (Evklid(deter3(tmp2), (*alphabet).size(), 3) == 1))
        ui->rec_hill_zashif->setEnabled(true);
    else
        ui->rec_hill_zashif->setEnabled(false);
}

void MainWindow::rec_hill_translate_ou(){
    QStandardItemModel *model1 = new QStandardItemModel;
    QStandardItemModel *model2 = new QStandardItemModel;
    QString temp1 = ui->rec_hill_key_ras->text();
    QString temp2 = ui->rec_hill_key_ras_2->text();
    while(temp1.size()!=9)
        temp1 += " ";
    while(temp2.size()!=9)
        temp2 += " ";
    int tmp1[3][3],tmp2[3][3];
    for(int i=0;i<9;i++){
        QStandardItem *item;
        int znac1 = search(temp1.at(i));
        int znac2 = search(temp2.at(i));
        tmp1[i/3][i%3] = znac1;
        tmp2[i/3][i%3] = znac2;
        item = new QStandardItem(QString::number(znac1,10));
        model1->setItem(i/3, i%3, item);
        item = new QStandardItem(QString::number(znac2,10));
        model2->setItem(i/3, i%3, item);
    }
    ui->rec_hill_table_ou->setModel(model1);
    ui->rec_hill_table_ou->resizeRowsToContents();
    ui->rec_hill_table_ou->resizeColumnsToContents();
    ui->rec_hill_table_ou_2->setModel(model2);
    ui->rec_hill_table_ou_2->resizeRowsToContents();
    ui->rec_hill_table_ou_2->resizeColumnsToContents();
    if(deter3(tmp1)!=0 && deter3(tmp2) != 0)
        ui->rec_hill_rashif->setEnabled(true);
    else
        ui->rec_hill_rashif->setEnabled(false);
}

void MainWindow::on_rec_hill_cl_in_clicked()
{
    ui->rec_hill_in->setText("");
}

void MainWindow::on_rec_hill_cl_ou_clicked()
{
    ui->rec_hill_ou->setText("");
}

void MainWindow::on_rec_hill_zashif_clicked()
{
    on_rec_hill_cl_ou_clicked();
    QString tmp = ui->rec_hill_key_zas->text();
    while(tmp.size()!=9)
        tmp += " ";
    for(int i=0;i<9;i++)
        key_i_2[i] = search(tmp.at(i));
    tmp = ui->rec_hill_key_zas_2->text();
    while(tmp.size()!=9)
        tmp += " ";
    for(int i=0;i<9;i++)
        key_i_1[i] = search(tmp.at(i));

    tmp = ui->rec_hill_in->text();
    while(tmp.size()%3 != 0)
        tmp += " ";
    *phrase = tmp;

    rec_hill_zashifrovanie();

    ui->rec_hill_ou->setText(*shifr);
}

void MainWindow::on_rec_hill_rashif_clicked()
{
    on_rec_hill_cl_in_clicked();
    QString tmp = ui->rec_hill_key_ras->text();
    while(tmp.size()!=9)
        tmp += " ";
    for(int i=0;i<9;i++)
        key_i_2[i] = search(tmp.at(i));
    tmp = ui->rec_hill_key_ras_2->text();
    while(tmp.size()!=9)
        tmp += " ";
    for(int i=0;i<9;i++)
        key_i_1[i] = search(tmp.at(i));

    tmp = ui->rec_hill_ou->text();
    while(tmp.size()%3 != 0)
        tmp += " ";
    *phrase = tmp;

    rec_hill_rashifrovanie();

    ui->rec_hill_in->setText(*shifr);
}

void MainWindow::rec_hill_zashifrovanie(){
    //tab_key - таблица с позициями символов из ключа
    //tab_phrase - таблица с позициями символов из фразы
    int tab_phrase[((*phrase).size()/3)][3];
    for(int i=0;i<(*phrase).size();i++)
        tab_phrase[i/3][i%3] = search((*phrase).at(i));
    int alf_size = (*alphabet).size();
    QString shifr_text;
    int tab_key[3][3];
    for(int n=0;n<((*phrase).size()/3);n++){
        //расчет ключей
        if(n>1){
            //остальные ключи
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    tab_key[i][j] = 0;
                    for(int k=0;k<3;k++)
                        tab_key[i][j] += key_i_2[i*3+k]*key_i_1[k*3+j];
                    tab_key[i][j] = tab_key[i][j] % alf_size;
                }
            }
            key_i_2 = key_i_1;
            for(int i=0;i<9;i++)
                key_i_1[i]=tab_key[i/3][i%3];
        }
        else{
            if(n==0){
                //ключ первый
                for(int i=0;i<9;i++)
                    tab_key[i/3][i%3] = key_i_2[i];
            }
            else{
                //ключ второй
                for(int i=0;i<9;i++)
                    tab_key[i/3][i%3] = key_i_1[i];
            }
        }
        //расчет ключей окончен
        for(int i=0;i<3;i++){
            int rez=0;
            for(int j=0;j<3;j++){
                rez += tab_key[i][j]*tab_phrase[n][j];
            }
            shifr_text += (*alphabet).at(rez % alf_size);
        }
    }
    *shifr = shifr_text;
}

void MainWindow::rec_hill_rashifrovanie(){
    //tab_key - таблица с позициями символов из ключа
    //tab_phrase - таблица с позициями символов из фразы
    int tab_phrase [((*phrase).size()/3)][3];
    for(int i=0;i<(*phrase).size();i++)
        tab_phrase[i/3][i%3] = search((*phrase).at(i));

    int alf_size = (*alphabet).size();
    QString shifr_text;
    int tab_key[3][3];
    for(int n=0;n<((*phrase).size()/3);n++){
        //расчет ключей
        if(n>1){
            //остальные ключи
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    tab_key[i][j] = 0;
                    for(int k=0;k<3;k++)
                        tab_key[i][j] += key_i_2[i*3+k]*key_i_1[k*3+j];
                    tab_key[i][j] = tab_key[i][j] % alf_size;
                }
            }
            key_i_2 = key_i_1;
            for(int i=0;i<9;i++)
                key_i_1[i]=tab_key[i/3][i%3];
        }
        else{
            if(n==0){
                //ключ первый
                for(int i=0;i<9;i++)
                    tab_key[i/3][i%3] = key_i_2[i];
            }
            else{
                //ключ второй
                for(int i=0;i<9;i++)
                    tab_key[i/3][i%3] = key_i_1[i];
            }
        }
        //расчет ключей окончен

        //получение обратной матрицы:
        int opred = deter3(tab_key);
        int x = Evklid(opred, alf_size, 1);
        if(opred>0){
            if(x<0)
                x = x + alf_size;
        }
        else{
            if(x<0)
                x = abs(x);
        }
        int reverse_key[3][3];

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                //получение элемента для матрицы алгебраических дополнений
                int tmp[2][2], stroka=0, stolbec=0;
                for(int ii=0;ii<3;ii++){
                    if(ii != i){
                        for(int jj=0;jj<3;jj++){
                            if(jj != j){
                                tmp[stroka][stolbec] = tab_key[ii][jj];
                                stolbec ++;
                            }
                        }
                        stroka ++;
                        stolbec = 0;
                    }
                }
                //
                reverse_key[j][i] = int(pow((-1),(i+j)))*(deter2(tmp));
                reverse_key[j][i] = (reverse_key[j][i]*x)%(alf_size);
                while(reverse_key[j][i]<0)
                    reverse_key[j][i] += alf_size;
            }
        }
        //получение обратной матрицы окончено

        for(int i=0;i<3;i++){
            int rez=0;
            for(int j=0;j<3;j++)
                rez += reverse_key[i][j]*tab_phrase[n][j];
            shifr_text += (*alphabet).at(rez % alf_size);
        }
    }
    *shifr = shifr_text;
}
//===================действия=с=рекуррентным=шифром=Хилла===========================

//===================действия=с=шифром=Виженера=====================================

void MainWindow::on_vizhener_zashifr_clicked()
{
    on_vizhener_cl_ou_clicked();
    QString vizhener_key = ui->vizhener_zas->text();
    QString vizhener_input = ui->vizhener_in->text();
    int alf_size = (*alphabet).size();

    if(ui->ciclic_key->isChecked()){
        QString gamma = vizhener_key;
        while(gamma.size()<vizhener_input.size())
            gamma += vizhener_key;
        int table_gamma[gamma.size()];
        for(int i=0;i<gamma.size();i++)
            table_gamma[i] = search(gamma.at(i));
        int table_input[vizhener_input.size()];
        for(int i=0;i<vizhener_input.size();i++)
            table_input[i] = search(vizhener_input.at(i));
        QString vizhener_shifr;
        for(int i=0;i<vizhener_input.size();i++)
            vizhener_shifr += (*alphabet).at((table_input[i] + table_gamma[i]) % alf_size);
        ui->vizhener_ou->setText(vizhener_shifr);
    }
    if(ui->with_self_key->isChecked()){
        QString gamma = vizhener_key + vizhener_input;
        int table_gamma[gamma.size()];
        for(int i=0;i<gamma.size();i++)
            table_gamma[i] = search(gamma.at(i));
        int table_input[vizhener_input.size()];
        for(int i=0;i<vizhener_input.size();i++)
            table_input[i] = search(vizhener_input.at(i));
        QString vizhener_shifr;
        for(int i=0;i<vizhener_input.size();i++)
            vizhener_shifr += (*alphabet).at((table_input[i] + table_gamma[i]) % alf_size);
        ui->vizhener_ou->setText(vizhener_shifr);
    }
}

void MainWindow::on_vizhener_rashifr_clicked()
{
    on_vizhener_cl_in_clicked();
    QString vizhener_key = ui->vizhener_ras->text();
    QString vizhener_input = ui->vizhener_ou->text();
    int alf_size = (*alphabet).size();

    if(ui->ciclic_key->isChecked()){
        QString gamma = vizhener_key;
        while(gamma.size()<vizhener_input.size())
            gamma += vizhener_key;
        int table_gamma[gamma.size()];
        for(int i=0;i<gamma.size();i++)
            table_gamma[i] = search(gamma.at(i));
        int table_input[vizhener_input.size()];
        for(int i=0;i<vizhener_input.size();i++)
            table_input[i] = search(vizhener_input.at(i));
        QString vizhener_text;
        for(int i=0;i<vizhener_input.size();i++){
            int rez = (table_input[i] + alf_size - table_gamma[i]) % alf_size;
            vizhener_text += (*alphabet).at(rez);
        }
        ui->vizhener_in->setText(vizhener_text);
    }
    if(ui->with_self_key->isChecked()){
        QString gamma = vizhener_key + vizhener_input;
        int table_gamma[gamma.size()];
        for(int i=0;i<vizhener_key.size();i++)
            table_gamma[i] = search(gamma.at(i));
        int table_input[vizhener_input.size()];
        for(int i=0;i<vizhener_input.size();i++)
            table_input[i] = search(vizhener_input.at(i));
        QString vizhener_text;
        for(int i=0;i<vizhener_input.size();i++){
            int rez = (table_input[i] + alf_size - table_gamma[i]) % alf_size;
            gamma += (*alphabet).at(rez);
            table_gamma[i+vizhener_key.size()] = rez;
            vizhener_text += (*alphabet).at(rez);
        }
        ui->vizhener_in->setText(vizhener_text);
    }
}

void MainWindow::on_vizhener_cl_in_clicked()
{
    ui->vizhener_in->setText("");
}

void MainWindow::on_vizhener_cl_ou_clicked()
{
    ui->vizhener_ou->setText("");
}

void MainWindow::check_vizhener(){
    if(ui->vizhener_zas->text() != ""){
        ui->vizhener_zashifr->setEnabled(true);
    }
    else{
        ui->vizhener_zashifr->setEnabled(false);
    }

    if(ui->vizhener_ras->text() != ""){
        ui->vizhener_rashifr->setEnabled(true);
    }
    else{
        ui->vizhener_rashifr->setEnabled(false);
    }
}
//===================действия=с=шифром=Виженера=====================================
