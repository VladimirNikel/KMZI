#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void count();

    void check();

    int deter2(int a[2][2]);

    int deter3(int a[3][3]);

    void hill_translate_in();

    void hill_translate_ou();

    void rec_hill_zashifrovanie();

    void rec_hill_rashifrovanie();

    void rec_hill_translate_in();

    void rec_hill_translate_ou();

    void check_vizhener();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString *alphabet = new QString;

    int search(QString a);

    int Evklid(int a, int b, int c);

private slots:
    void on_aff_zashif_clicked();

    void on_aff_cl_in_clicked();

    void on_aff_cl_ou_clicked();

    void on_aff_rashif_clicked();

    void on_aff_rec_zashif_clicked();

    void on_aff_rec_cl_in_clicked();

    void on_aff_rec_cl_ou_clicked();

    void on_aff_rec_rashif_clicked();

    void on_pushButton_clicked();

    void on_hill_cl_in_clicked();

    void on_hill_cl_ou_clicked();

    void on_hill_zashif_clicked();

    void on_hill_rashif_clicked();

    void multiplication();

    void remultiplication();

    void on_rec_hill_cl_in_clicked();

    void on_rec_hill_cl_ou_clicked();

    void on_rec_hill_zashif_clicked();

    void on_rec_hill_rashif_clicked();

    void on_vizhener_zashifr_clicked();

    void on_vizhener_rashifr_clicked();

    void on_vizhener_cl_in_clicked();

    void on_vizhener_cl_ou_clicked();

private:
    Ui::MainWindow *ui;

    QString *key      = new QString;
    QString *phrase   = new QString;
    QString *shifr    = new QString;

    int *key_i_2 = new int[9];
    int *key_i_1 = new int[9];
};

#endif // MAINWINDOW_H
