#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    //void on_actionNew_File_triggered();

    void on_ajout_clicked();

    void on_pushButton_2_clicked();

    bool isNumericText(QString);

    bool isLetters(QString);

    void on_menu_currentChanged(int index);

    void on_load_file_clicked();

    void on_actualiser_liste_inscripion_clicked();

    void on_annee_liste_inscrits_textChanged(const QString &arg1);

    void on_niveau_liste_inscrits_currentTextChanged(const QString &arg1);

    void on_parcours_liste_inscrits_currentTextChanged(const QString &arg1);

    void on_edit_parcours_4_textChanged(const QString &arg1);

    void on_table_inscription_liste_clicked(const QModelIndex &index);

    void on_modifier_liste_clicked();

    void on_supprimer_liste_clicked();

    void on_pushButton_clicked();

    void on_modif_btn_confirmer_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_11_clicked();

    void on_fiche_de_pres_tableVW_clicked(const QModelIndex &index);

    void actualize_fiche_de_pres_table(QString);

    void on_pushButton_3_clicked();

    void on_decompte_anne_textChanged(const QString &arg1);

    void on_presence_acceuil_clicked();

    void on_decompte_acceuil_clicked();

    void actualize_tableau_d_enseignement();
    void on_pushButton_19_clicked();

    void on_critere_de_rech_currentIndexChanged(int index);

    void on_pushButton_5_clicked();

    void on_decompte_parcours_currentIndexChanged(int index);

    void on_decompte_niveau_currentIndexChanged(int index);

    void on_pushButton_20_clicked();

    void on_modif_ajout_btn_ok_clicked();

    void on_modif_ajout_fichePres_stateChanged(int arg1);

    void on_pushButton_12_clicked();

    void on_enseignement_tableVW_clicked(const QModelIndex &index);

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_consult_fichePres_date_currentTextChanged(const QString &arg1);

    void actualize_table_consult_fichePres();

    void actualize_comboBox_date_consult_fichePres();

    void on_consult_fichePres_parcours_currentTextChanged(const QString &arg1);

    void on_consult_fichePres_niveau_currentTextChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_presence_acceuil_2_clicked();

    void on_enseignement_liste_acceuil_clicked();

    void on_enseignement_ajout_acceuil_clicked();

    void on_enseignement_modif_acceuil_clicked();

    void on_enseignement_suppr_acceuil_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    bool firstUse();

    void setFirstUse(bool);

    QString password();

    void setPassword(QString);

    void ChangePassword();

    void on_password_btn_clicked();

    void on_password_textChanged(const QString &arg1);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    bool etudiantExiste(QString ,QString ,QString);

    void on_pushButton_15_clicked();

private:
    Ui::MainWindow *ui;
    QString etudiantSelect;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QString databaseLocation;
    QString enseignementSelect;
    QString enseignementSelectType;
    QString enseignementSelectMatiere;
    bool ajout;
};

#endif // MAINWINDOW_H
