#include "mainwindow.h"
#include "ui_mainwindow.h"


//------------------constructeur de le fenetre(etats det variables par defauts)------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setMinimumWidth(1050);
    ui->menu->setCurrentIndex(0);
    ui->menuBar->setVisible(false);
    ui->stack_liste->setCurrentWidget(ui->page_table);
    databaseLocation="/media/tsilavo/Disque local1/ENI/projet_fin_d_annee/Ubuntu/Gestion_abscences/etudiant";

    ui->pres_date->setDate(QDate::currentDate());

    ui->stack_ens->setCurrentWidget(ui->stack_table_enseignement);

    //--------------------------------------------------------------------
    ui->stackedWidget->setCurrentIndex(1);
    ui->password_incorrect->setVisible(false);
    ui->password_incorrect_2->setVisible(false);
    ui->password_btn->setEnabled(false);
    ui->password_label_2->setVisible(false);
    if(firstUse()){
        ui->password_label_2->setVisible(true);
        ui->password->setEchoMode(QLineEdit::Normal);
        ui->password_label->setText("Choisir un mot de passe pour l'application:");
    }
    ui->stackedWidget_8->setCurrentIndex(0);
    connect(ui->actionChanger_de_mot_de_passe,&QAction::triggered,this,&MainWindow::ChangePassword);
}

//------------------destructeur de la fenetrer------------------
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_password_textChanged(const QString &arg1)
{
    if(arg1=="")ui->password_btn->setEnabled(false);
    else ui->password_btn->setEnabled(true);
}

void MainWindow::on_password_btn_clicked()
{
    if(firstUse()){
        ui->password->setEchoMode(QLineEdit::Normal);
        ui->password_label->setText("Choisir un mot de passe pour l'application:");
        setPassword(ui->password->text());
        QMessageBox::information(this,"Succées","Mot de passe ajouté");
        setFirstUse(false);
        ui->stackedWidget->setCurrentIndex(0);
    }else{
        ui->password_label_2->setVisible(false);
        ui->password_label->setText("Entrer le mot de passe:");
        ui->password->setEchoMode(QLineEdit::Password);
        if (password()!=ui->password->text()) {
            ui->password_incorrect->setVisible(true);
            ui->password->setText("");
        }else{
            ui->stackedWidget->setCurrentIndex(0);
            ui->password_incorrect->setVisible(false);
            ui->stackedWidget->setCurrentIndex(0);
        }

    }

}

void MainWindow::ChangePassword(){
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_8->setCurrentIndex(1);
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1==1)ui->menuBar->setVisible(false);
    else ui->menuBar->setVisible(true);
}

bool MainWindow::firstUse(){
    bool first(false);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        q.prepare("select first_use from securite");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            while (q.next()) {
                if(q.value(0).toString()=="true")first=true;
                break;
            }
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
    return first;
}

void MainWindow::setFirstUse(bool first){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        if(first)q.prepare("update securite set first_use='true'");
        else q.prepare("update securite set first_use='false'");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}

QString MainWindow::password(){
    QString passwd;
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        q.prepare("select MDP from securite");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
        }else{
            while (q.next()) {
                passwd=q.value(0).toString();
                break;
            }
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
    return passwd;
}

void MainWindow::setPassword(QString mdp){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        q.prepare("update securite set MDP='"+mdp+"'");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}
//------------annuler le changement de mot de passe-----------------------------
void MainWindow::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//------------confirmer le changement de mot de passe-----------------------------
void MainWindow::on_pushButton_14_clicked()
{
    if(password()!=ui->password_courant->text()){
        ui->password_incorrect_2->setVisible(true);
        ui->password_nouveau->setText("");
        ui->password_courant->setText("");
    }else{
        ui->password_incorrect_2->setVisible(false);
        setPassword(ui->password_nouveau->text());
        ui->password_nouveau->setText("");
        ui->password_courant->setText("");
        ui->stackedWidget->setCurrentIndex(0);
    }
}
//------------------fonction de verifier si un Qstring particulier est un texte numerique------------------
bool MainWindow::isNumericText(QString text)
{
    bool valid(true);
    for(int i(0);i<text.length();i++){
        if((text[i]<'0')||(text[i]>'9')) valid=false;
    }
    return valid;
}

//------------------fonction de verifier si un Qstring particulier est un texte alphabetique------------------
bool MainWindow::isLetters(QString text){
     bool valid(true);
     for(char i('0');i<='9';i++){
         if(text.contains(i)) valid=false;
     }
     return valid;
}

//------------------quand on clique sur ajout dans l'onglet liste------------------
void MainWindow::on_ajout_clicked()
{
    ui->menu->setCurrentWidget(ui->inscription);
}

//------------------quand on clique sur valid dans l'onglet inscription------------------
void MainWindow::on_pushButton_2_clicked()
{
    bool valid(false);
    if(
            (isLetters(ui->nom->text()))&&
            (ui->nom->text()!="")&&
            (isLetters(ui->prenom->text()))&&
            (ui->prenom->text()!="")&&
            (isLetters(ui->lieuNaiss->text()))&&
            (ui->lieuNaiss->text()!="")&&
            (isNumericText(ui->numBac->text()))&&
            (ui->numBac->text()!="")&&
            (ui->Ninscription->text()!="")&&
            (isNumericText(ui->annee->text()))&&
            (ui->annee->text()!="")
        ){
        valid=true;
    }
    if(etudiantExiste(ui->Ninscription->text(),ui->nom->text(),ui->prenom->text())){
        QMessageBox::information(this,"existe dejà","Etudiant déjà saisit");
        valid=false;
        ui->pushButton_4->click();
    }
    if(valid){
        //------------------connection au base de donnee------------------
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(database.open()){
            //QMessageBox::information(this,"connected","base de données connecté");
            //entrer les valeurs dans la base de données
            QSqlQuery q;
            q.prepare("insert into t_etudiant values (:numero,:nom,:prenom,:dateNaiss,:lieuNaiss,:annee,:numBac,:serie,:parcours,:niveau)");
            q.bindValue(":numero",ui->Ninscription->text());
            q.bindValue(":nom",ui->nom->text());
            q.bindValue(":prenom",ui->prenom->text());
            q.bindValue(":lieuNaiss",ui->lieuNaiss->text());
            q.bindValue(":annee",ui->annee->text());
            q.bindValue(":numBac",ui->numBac->text());
            q.bindValue(":dateNaiss",ui->dateNaiss->date());
            q.bindValue(":serie",ui->serie->currentText());
            q.bindValue(":parcours",ui->parcours->currentText());
            q.bindValue(":niveau",ui->niveau->currentText());
            if(!q.exec()){
                QMessageBox::warning(this,"execution failure","unable to execute querry");
                qDebug()<<q.lastError();
            }else{
                QMessageBox::information(this,"succées","etudiant ajoutée");
                on_pushButton_4_clicked();
            }
        }else{
            QMessageBox::warning(this,"error"," base de données non connecté");
            qDebug()<<database.lastError();
        }
    }else{
        QMessageBox::warning(this,"Erreur","informations non valide");
    }
}
//-------------------VERIFIER SI  L'ETUDIANT EXISTE DEJÀ-----------------
bool MainWindow::etudiantExiste(QString matricule,QString nom,QString prenom){
    bool existe(false);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        //entrer les valeurs dans la base de données
        QSqlQuery q;
        q.prepare("select et_numero from t_etudiant where et_numero='"+matricule+"'");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            while (q.next()) {
                if(q.value(0).toString()==matricule) return true;
                else existe = false;
                break;
            }
        }

        q.prepare("select et_nom||' '||et_prenom from t_etudiant where et_nom='"+nom+"' and et_prenom='"+prenom+"'");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            while (q.next()) {
                if(q.value(0).toString()==(nom+" "+prenom)) return true;
                else existe = false;
                break;
            }
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
    return existe;
}
//-------------------Annuler l'ajout--------------------------------------
void MainWindow::on_pushButton_4_clicked()
{
    ui->Ninscription->setText("");
    ui->nom->setText("");
    ui->prenom->setText("");
    ui->lieuNaiss->setText("");
    ui->annee->setText("");
    ui->numBac->setText("");
    ui->dateNaiss->setDate(QDate::currentDate());
    ui->serie->setCurrentText("");
    ui->parcours->setCurrentText("");
    ui->niveau->setCurrentText("");
}

//------------------importer les donnes depuis un fichier .csv------------------
void MainWindow::on_load_file_clicked()
{   QString chemin = QFileDialog::getOpenFileName(this,"importer les donnees","CSV File(*.csv)");
    ui->chemin_csv->setText(chemin);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QMessageBox::information(this,"connected","base de données connecté");
        //entrer les valeurs dans la base de données
        QSqlQuery q;
        q.prepare("load data local infile '"+chemin+"' into table t_etudiant fields terminated by ',' enclosed by '\"' lines terminated by '\\n' ignore 1 rows;");
        if(!q.exec()){
            QMessageBox::warning(this,"execution failure","unable to execute querry");
            qDebug()<<q.lastError();
        }else{
            QMessageBox::information(this,"success","data inserted");
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}

//------------------Quand on change d'onglet------------------
void MainWindow::on_menu_currentChanged(int index)
{
    if(index==1){
        this->setMinimumWidth(1000);
        this->setMinimumHeight(700);
    }else if((index==0)||(index==2)){
        this->setMinimumWidth(1050);
    }else if(index==2){
        ui->actualiser_liste_inscripion->click();
    }else if(index==4){
        actualize_comboBox_date_consult_fichePres();
        actualize_table_consult_fichePres();
    }else if(index==5){
        ui->pushButton_5->click();
    }else if(index==6){
        actualize_tableau_d_enseignement();
    }else{
        this->setMinimumWidth(0);
        this->setMinimumHeight(0);
    }
}


//-----------------------Actualuser la table 'Liste d'inscription'--------------------------------------------
void MainWindow::on_actualiser_liste_inscripion_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQueryModel *qm = new QSqlQueryModel(this);
        QSqlQuery q;
        q.prepare("select * from t_etudiant where et_niveau=\""+ui->niveau_liste_inscrits->currentText()+"\" and et_parcours=\""+ui->parcours_liste_inscrits->currentText()+"\" and et_anne="+ui->annee_liste_inscrits->text()+"");
        qm->setQuery(q.lastQuery());
        ui->table_inscription_liste->setModel(qm);
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}

//-----------------------Actualuser la table 'Liste d'inscription'--------------------------------------------
void MainWindow::on_annee_liste_inscrits_textChanged(const QString &arg1)
{
    ui->annee_liste_inscrits->setText(arg1);
    ui->actualiser_liste_inscripion->click();
}


//-----------------------Actualuser la table 'Liste d'inscription'--------------------------------------------
void MainWindow::on_niveau_liste_inscrits_currentTextChanged(const QString &arg1)
{
    ui->niveau_liste_inscrits->setCurrentText(arg1);
    ui->actualiser_liste_inscripion->click();
}


//-----------------------Actualuser la table 'Liste d'inscription'--------------------------------------------
void MainWindow::on_parcours_liste_inscrits_currentTextChanged(const QString &arg1)
{
    ui->parcours_liste_inscrits->setCurrentText(arg1);
    ui->actualiser_liste_inscripion->click();
}

//------------------rechercher un etudiant par son Nom ou matricule------------------
void MainWindow::on_edit_parcours_4_textChanged(const QString &arg1)
{
    QString mode = (ui->critere_de_rech->currentText()=="Nom")? "et_nom":"et_numero";
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQueryModel *qm = new QSqlQueryModel(this);
        QSqlQuery q;
        QString str1("select * from t_etudiant where et_niveau=\""+ui->niveau_liste_inscrits->currentText()+"\" and et_parcours=\""+ui->parcours_liste_inscrits->currentText()+"\" and et_anne="+ui->annee_liste_inscrits->text()+" and "+mode+" like \"%"+arg1+"%\"");
        QString str2("select * from t_etudiant where "+mode+" like \"%"+arg1+"%\"");
        (ui->plus_de_criteres->isChecked())?q.prepare(str1):q.prepare(str2);
        qm->setQuery(q.lastQuery());
        ui->table_inscription_liste->setModel(qm);
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}
//------------------------cliquer le boutton rechercher--------------------------------
void MainWindow::on_pushButton_15_clicked()
{
    bool trouve(false);
    QString mode = (ui->critere_de_rech->currentText()=="Nom")? "et_nom":"et_numero";
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        QString str1("select * from t_etudiant where et_niveau=\""+ui->niveau_liste_inscrits->currentText()+"\" and et_parcours=\""+ui->parcours_liste_inscrits->currentText()+"\" and et_anne="+ui->annee_liste_inscrits->text()+" and "+mode+"='"+ui->edit_parcours_4->text()+"'");
        QString str2("select * from t_etudiant where "+mode+"='"+ui->edit_parcours_4->text()+"'");
        (ui->plus_de_criteres->isChecked())?q.prepare(str1):q.prepare(str2);
        if(!q.exec()){
            QMessageBox::warning(this,"Error","Unable to execute Querry : " + q.lastQuery());
            qDebug()<<q.lastError();
        }else{
            while(q.next()){
                if(q.value(0).toString()!=""){
                    trouve=true;
                    break;
                }
            }
            if(!trouve){
                QMessageBox::information(this,"Non trouvé","Etudiant non trouvé");
            }
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}

//------------------------Qand on change les criteres de recherches---------------------
void MainWindow::on_critere_de_rech_currentIndexChanged(int index)
{
    ui->critere_de_rech->setCurrentIndex(index);
    on_edit_parcours_4_textChanged(ui->edit_parcours_4->text());
}

//----------------------Selectionner un etudiant dans la liste d'inscription----------------------------
void MainWindow::on_table_inscription_liste_clicked(const QModelIndex &index)
{
    ui->table_inscription_liste->selectRow(index.row());
    QModelIndex indexMatricule(index.siblingAtColumn(0));
    etudiantSelect = ui->table_inscription_liste->model()->itemData(indexMatricule)[0].toString();
    //activer les bouttons modifier et supprimer
        ui->modifier_liste->setEnabled(true);
        ui->supprimer_liste->setEnabled(true);
}

//------------------Quand on clique sur mdifier dans l'onglet Liste des Inscrits------------------
void MainWindow::on_modifier_liste_clicked()
{
    if(etudiantSelect==""){
        QMessageBox::warning(this,"Erreur","Aucun etudiant selectionnées!!!");
        ui->stack_liste->setCurrentWidget(ui->page_table);
    }else{
        ui->stack_liste->setCurrentWidget(ui->page_modifier);
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(!database.open()){
            QMessageBox::warning(this,"error"," base de données non connecté");
            qDebug()<<database.lastError();
        }else{
            //------UTILISER LE NUMERO MATRICULE POUR RECUPERER LES INFORMATTIONS SUR L'ETUDIANT------
            QSqlQuery q("select * from t_etudiant where et_numero="+etudiantSelect);
            if(!q.exec()){
                QMessageBox::warning(this,"Error","Unable to execute Querry : " + q.lastQuery());
                qDebug()<<q.lastError();
            }else{
                //------recuperer les informations sur l'etudiant a partir de la base de données------
                while (q.next()) {
                    ui->modif_Ninscription->setText(etudiantSelect);
                    ui->modif_nom->setText(q.value(1).toString());
                    ui->modif_prenom->setText(q.value(2).toString());
                    ui->modif_dateNaiss->setDate(q.value(3).toDate());
                    ui->modif_lieuNaiss->setText(q.value(4).toString());
                    ui->modif_annee->setText(q.value(5).toString());
                    ui->modif_numBac->setText(q.value(6).toString());
                    ui->modif_serie->setCurrentText(q.value(7).toString());
                    ui->modif_parcours->setCurrentText(q.value(8).toString());
                    ui->modif_niveau->setCurrentText(q.value(9).toString());
                }
            }
        }
    }
}

//------------------Quand on Confirme la modification------------------
void MainWindow::on_modif_btn_confirmer_clicked()
{
    bool valid(false);
    if(
            (isLetters(ui->modif_nom->text()))&&
            (ui->modif_nom->text()!="")&&
            (isLetters(ui->modif_prenom->text()))&&
            (ui->modif_prenom->text()!="")&&
            (isLetters(ui->modif_lieuNaiss->text()))&&
            (ui->modif_lieuNaiss->text()!="")&&
            (isNumericText(ui->modif_numBac->text()))&&
            (ui->modif_numBac->text()!="")&&
            (ui->modif_Ninscription->text()!="")&&
            (isNumericText(ui->modif_annee->text()))&&
            (ui->modif_annee->text()!="")
        ){
        valid=true;
    }
    if(valid){
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(database.open()){
            //QMessageBox::information(this,"connected","base de données connecté");
            //entrer les valeurs dans la base de données
            QSqlQuery q;
            //-----------------------------MODIFER DANS LA LISTE DES ABSCENCES----------------------------------------------------
            q.prepare("update t_absent set abs_numero=:numero, abs_niveau=:niveau,abs_annee=:annee where abs_numero="+etudiantSelect);
            q.bindValue(":numero",ui->modif_Ninscription->text());
            q.bindValue(":niveau",ui->modif_niveau->currentText());
            q.bindValue(":annee",ui->modif_annee->text());
            if(!q.exec()){
                QMessageBox::warning(this,"execution failure","unable to execute querry");
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }
            //-----------------------------MODIFIER DANS LA LISTE DES ABSCENCES----------------------------------------------------

            //-----------------------------SUPPRIMER DANS LA LISTE DES ETUDIANT----------------------------------------------------
            q.prepare("update t_etudiant set et_numero=:numero, et_nom=:nom, et_prenom=:prenom, et_dateNaiss=:dateNaiss, \
                        et_lieuNaiss=:lieuNaiss, et_anne=:annee, et_numBac=:numBac, et_serieBac=:serie, et_parcours=:parcours, \
                        et_niveau=:niveau where et_numero="+etudiantSelect);
            q.bindValue(":numero",ui->modif_Ninscription->text());
            q.bindValue(":nom",ui->modif_nom->text());
            q.bindValue(":prenom",ui->modif_prenom->text());
            q.bindValue(":lieuNaiss",ui->modif_lieuNaiss->text());
            q.bindValue(":annee",ui->modif_annee->text());
            q.bindValue(":numBac",ui->modif_numBac->text());
            q.bindValue(":dateNaiss",ui->modif_dateNaiss->date());
            q.bindValue(":serie",ui->modif_serie->currentText());
            q.bindValue(":parcours",ui->modif_parcours->currentText());
            q.bindValue(":niveau",ui->modif_niveau->currentText());
            if(!q.exec()){
                QMessageBox::warning(this,"execution failure","unable to execute querry");
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }else{
                QMessageBox::information(this,"succés","informations sur l'etudiant modifié");
                //--------MODIFICATION TERMINEE-----
                //--------désactiver les bouttons modifier et supprimer et remetre a nulle la variable etudiantSelect----
                    etudiantSelect="";
                    ui->table_inscription_liste->clearSelection();
                    ui->modifier_liste->setEnabled(false);
                    ui->supprimer_liste->setEnabled(false);
                    ui->stack_liste->setCurrentWidget(ui->page_table);
                    ui->actualiser_liste_inscripion->click();
            }
        }else{
            QMessageBox::warning(this,"error"," base de données non connecté");
            qDebug()<<database.lastError();
        }
    }else{
        QMessageBox::warning(this,"Erreur","informations non valide");
    }
}

//------------------Quand on annule la modification------------------
void MainWindow::on_pushButton_clicked()
{
    ui->stack_liste->setCurrentWidget(ui->page_table);
}


//------------------Quand on clique sur supprimer dans l'onglet Liste des Inscrits------------------
void MainWindow::on_supprimer_liste_clicked()
{
    if(etudiantSelect!=""){
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(database.open()){
            QSqlQuery q;

            //-----------------------------SUPPRIMER DANS LA LISTE DES ABSCENCES----------------------------------------------------
            q.prepare("delete from t_etudiant where et_numero="+etudiantSelect);
            if(!q.exec()){
                QMessageBox::warning(this,"execution failure","unable to execute querry");
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }
            //-----------------------------SUPPRIMER DANS LA LISTE DES ABSCENCES----------------------------------------------------

            q.prepare("delete from t_etudiant where et_numero="+etudiantSelect);
            if(q.exec()){
                etudiantSelect="";
                ui->table_inscription_liste->clearSelection();
                //désactiver les bouttons modifier et supprimer
                    ui->modifier_liste->setEnabled(false);
                    ui->supprimer_liste->setEnabled(false);
                QMessageBox::information(this,"Supprimé","Eleve Supprimée de la base de données");
            }else{
                QMessageBox::warning(this,"Erreur","Impossible de supprimer l'eleve");
                qDebug()<<q.lastError();
            }
        }else{
            QMessageBox::warning(this,"error"," base de données non connecté");
            qDebug()<<database.lastError();
        }
    }
    ui->actualiser_liste_inscripion->click();
}

//------------------Ajouter depuis l'onglet Acceuil------------------
void MainWindow::on_pushButton_7_clicked()
{
    ui->menu->setCurrentIndex(1);
}

//------------------Consulter la liste depuis l'onglet Acceuil------------------
void MainWindow::on_pushButton_8_clicked()
{
    ui->menu->setCurrentIndex(2);
}

//------------------Modifier un etudiant depuis l'onglet Acceuil------------------
void MainWindow::on_pushButton_9_clicked()
{
     ui->menu->setCurrentIndex(2);
     QMessageBox::information(this,"Modifier","1°- Rechercher puis selectioner(cliquer sur) l'etudiant a modifier\n"
                                              "2°- Appuiez sur le boutton 'modifier' en bas\n"
                                              "3°- Effectuer les modifications dans la page affiché\n"
                                              "4°- Cliquer sur 'OK' pour confirmer , sur 'Annuler' pour annuler\n");
}

//------------------Supprimer un etudiant depuis l'onglet Acceuil------------------
void MainWindow::on_pushButton_10_clicked()
{
     ui->menu->setCurrentIndex(2);
     QMessageBox::information(this,"Modifier","1°- Rechercher puis selectioner(cliquer sur) l'etudiant a modifier\n"
                                              "2°- Appuiez sur le boutton 'supprimer' en bas");
}

//------------------------------------------------dans l'onglet Fiche de presence 'Realiser un appel'.onclick---------------------------------------------------------
void MainWindow::on_pushButton_11_clicked()
{
    bool valid(true);
    QString matricule;
    QString niveau=ui->pres_niveau->currentText();
    QString parcours=ui->pres_parcours->currentText();
    QString annee; annee.setNum(ui->pres_date->date().year());
    QDate date = ui->pres_date->date();
    QString ens_numero;
    QString ens_type;

    if((ui->enseignant->text()=="")||(ui->matiere->text()==""))valid=false;
    if(valid){
        ui->pushButton_11->setEnabled(false);
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(!database.open()){
            QMessageBox::warning(this,"erreur","base de données non connectée");
            qDebug()<<database.lastError();
        }else{
            QSqlQuery q;
            q.prepare("insert into t_enseignement values (null,'"+ui->typeEns->currentText()+"','"+ui->matiere->text()+"')");
            if(!q.exec()){
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }else{
               //actualize_fiche_de_pres_table();
            }

            q.prepare("select ens_numero, ens_type from t_enseignement order BY ens_numero desc limit 1");
            if(!q.exec()){
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }else{
                while (q.next()) {
                    ens_numero=q.value(0).toString();
                    ens_type=q.value(1).toString();
                    qDebug()<<"ens_numero: "+ens_numero;
                    qDebug()<<"ens_type: "+ens_type;
                }
            }

           q.prepare("select et_numero from t_etudiant where et_parcours='"+parcours+"' and et_niveau='"+niveau+"'");
           if(!q.exec()){
               qDebug()<<q.lastError();
               qDebug()<<q.lastQuery();
           }else{
                while (q.next()) {
                    matricule = q.value(0).toString();
                    qDebug()<<matricule;
                    QSqlQuery q1;
                    q1.prepare("insert into t_absent values (:matricule,:niveau,:annee,:ens_numero,:ens_type,:date,null)");
                    q1.bindValue(":matricule",matricule);
                    q1.bindValue(":niveau",niveau);
                    q1.bindValue(":annee",annee);
                    q1.bindValue(":ens_numero",ens_numero);
                    q1.bindValue(":ens_type",ens_type);
                    q1.bindValue(":date",date);
                    if(!q1.exec()){
                        qDebug()<<q1.lastError();
                        qDebug()<<q1.lastQuery();
                    }
                }
                actualize_fiche_de_pres_table(ens_numero);
            }

        }
    }
}
//------------------------------------------------actualiser la fiche de presence---------------------------------------------------------------------
void MainWindow::actualize_fiche_de_pres_table(QString ens_numero){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(!database.open()){
        QMessageBox::warning(this,"erreur","base de données non connectée");
        qDebug()<<database.lastError();
    }else{
        QSqlQuery q;
        q.prepare("select t_absent.abs_absent as ABSENT, t_etudiant.et_numero, t_etudiant.et_nom||' '||t_etudiant.et_prenom as 'Nom & Prenom', t_absent.abs_numero_ens as 'Enseignement N°', t_etudiant.et_niveau as Niveau,t_etudiant.et_parcours as Parcours\
                  from t_absent left join t_etudiant on t_etudiant.et_numero=t_absent.abs_numero \
                  where t_absent.abs_numero_ens="+ens_numero+"");
            if(!q.exec()){
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }else{
            QSqlQueryModel *qm =new QSqlQueryModel(this);
            qm->setQuery(q.lastQuery());
            ui->fiche_de_pres_tableVW->setModel(qm);
            ui->fiche_de_pres_tableVW->setColumnWidth(2,500);
            ui->fiche_de_pres_tableVW->setColumnWidth(3,250);
        }
    }
}

//------------------------------------------------Quand on clique dans la table Fiche de presence---------------------------------------------------------
void MainWindow::on_fiche_de_pres_tableVW_clicked(const QModelIndex &index)
{
    QString matricule = ui->fiche_de_pres_tableVW->model()->itemData(index.siblingAtColumn(1))[0].toString();
    QString ens_numero=ui->fiche_de_pres_tableVW->model()->itemData(index.siblingAtColumn(3))[0].toString();
    qDebug()<<matricule;

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(!database.open()){
        QMessageBox::warning(this,"erreur","base de données non connectée");
    }else{
        QSqlQuery q;
        q.prepare("select abs_absent from t_absent where abs_numero="+matricule+" and abs_numero_ens='"+ens_numero+"'");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
             while (q.next()) {
                QString absent = q.value(0).toString();
                if(absent=="ABS"){
                    QSqlQuery q1;
                    q1.prepare("update t_absent set abs_absent=null where abs_numero="+matricule+" and abs_numero_ens='"+ens_numero+"'");
                    if(!q1.exec()){
                        qDebug()<<q1.lastError();
                        qDebug()<<q1.lastQuery();
                    }else break;
                }else{
                    QSqlQuery q1;
                    q1.prepare("update t_absent set abs_absent='ABS' where abs_numero="+matricule+" and abs_numero_ens='"+ens_numero+"'");
                    if(!q1.exec()){
                        qDebug()<<q1.lastError();
                        qDebug()<<q1.lastQuery();
                    }else break;
                }
             }
             actualize_fiche_de_pres_table(ens_numero);
             ui->fiche_de_pres_tableVW->selectRow(index.row());
         }
    }
}

//------------------------------------------------Confirmer la fiche de presence apres l'Appel des etudiants---------------------------------------------------------
void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_11->setEnabled(true);
    QSqlQueryModel *qm = new QSqlQueryModel(this);
    ui->fiche_de_pres_tableVW->setModel(qm);
    ui->matiere->setText("");
    ui->enseignant->setText("");
}

//------------------------------------------------decompte des abscences------------------------------------------------
void MainWindow::on_decompte_anne_textChanged(const QString &arg1)
{
    ui->decompte_anne->setText(arg1);
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(!database.open()){
        QMessageBox::warning(this,"erreur","base de données non connectée");
        qDebug()<<database.lastError();
    }else{
        QSqlQuery q;
        q.prepare("select t_etudiant.et_numero as 'N° Inscription',"
                  " t_etudiant.et_nom ||' '|| t_etudiant.et_prenom as 'Nom & Prenom',"
                  "count(abs_absent) as \"Nombre d'absence en "+ui->decompte_anne->text()+"\""
                  "from t_etudiant left join t_absent on t_etudiant.et_numero=t_absent.abs_numero "
                  "where t_etudiant.et_parcours='"+ui->decompte_parcours->currentText()+"'and t_etudiant.et_niveau='"+ui->decompte_niveau->currentText()+"' and t_etudiant.et_anne='"+ui->decompte_anne->text()+"'"
                  "group by et_numero");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            qDebug()<<q.lastQuery();
            QSqlQueryModel *qm = new QSqlQueryModel(this);
            qm->setQuery(q.lastQuery());
            ui->decompte_abs_tableVW->setModel(qm);
            ui->decompte_abs_tableVW->setColumnWidth(1,500);
            ui->decompte_abs_tableVW->setColumnWidth(2,300);
        }
    }
}
//-----------------------------------------Actualiser le table de decompte------------------------------------------------
void MainWindow::on_pushButton_5_clicked()
{
    on_decompte_anne_textChanged(ui->decompte_anne->text());
}

//-----------------------------------------Actualiser le table de decompte------------------------------------------------
void MainWindow::on_decompte_parcours_currentIndexChanged(int index)
{
    ui->decompte_parcours->setCurrentIndex(index);
    ui->pushButton_5->click();
}

//-----------------------------------------Actualiser le table de decompte------------------------------------------------
void MainWindow::on_decompte_niveau_currentIndexChanged(int index)
{
    ui->decompte_niveau->setCurrentIndex(index);
    ui->pushButton_5->click();
}


//----------------------choisir remplir une fiche de presence dans l'acceuil-------------------------------
void MainWindow::on_presence_acceuil_clicked()
{
    ui->menu->setCurrentIndex(3);
}

//----------------------choisir consulter fiche de presence d'abscence dans l'acceuil-------------------------------
void MainWindow::on_presence_acceuil_2_clicked()
{
    ui->menu->setCurrentIndex(4);
}

//----------------------choisir decompte d'abscence dans l'acceuil-------------------------------
void MainWindow::on_decompte_acceuil_clicked()
{
    ui->menu->setCurrentIndex(5);
}

//-------------------------------actualiser le tableau d'enseignement----------------------------
void MainWindow::actualize_tableau_d_enseignement(){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(!database.open()){
        QMessageBox::warning(this,"erreur","base de données non connectée");
        qDebug()<<database.lastError();
    }else{
        QSqlQuery q;
        q.prepare("select t_enseignement.ens_numero as \"N° d'enseignement\",t_enseignement.ens_nom as 'Matiere',t_enseignement.ens_type as 'type',"
                  "abs_date,abs_niveau,"
                  "t_etudiant.et_parcours "
                  "from t_absent "
                  "left join t_enseignement on t_enseignement.ens_numero=t_absent.abs_numero_ens "
                  "inner join t_etudiant on t_absent.abs_numero=t_etudiant.et_numero "
                  "group by t_absent.abs_numero_ens");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            QSqlQueryModel *qm = new QSqlQueryModel(this);
            qm->setQuery(q.lastQuery());
            ui->enseignement_tableVW->setModel(qm);
            ui->enseignement_tableVW->setColumnWidth(0,300);
        }
    }
}

void MainWindow::on_pushButton_19_clicked()
{
    actualize_tableau_d_enseignement();
}
//-------------------- AJOUTER UN NOUVEL ENSEINGNEMENT--------------------------------
void MainWindow::on_pushButton_20_clicked()
{
    ui->stack_ens->setCurrentWidget(ui->stack_ajouter_modifier_enseignement);
    ui->modif_ajout_titre->setText("AJOUTER UN ENSEIGNEMENT");
    ajout=true;
    ui->pushButton_19->click();
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
}

//---------------------CONFIRMER L'AJOUT ou MODIFIER D'UN NOUVEL ENSEIGNEMENT---------------------
void MainWindow::on_modif_ajout_btn_ok_clicked()
{
    if(ajout){//----------------------------------------------------------------------------ajoiter
        ui->modif_ajout_fichePres->setEnabled(true);
        if(!(ui->modif_ajout_matiere->text()=="")){
            if(ui->modif_ajout_fichePres->isChecked()){
                ui->menu->setCurrentIndex(3);
                ui->pres_date->setDate(ui->dateEdit->date());
                ui->pres_niveau->setCurrentText(ui->modif_ajout_niveau->currentText());
                ui->pres_parcours->setCurrentText(ui->modif_ajout_parcours->currentText());
                ui->matiere->setText(ui->modif_ajout_matiere->text());
                ui->enseignant->setText(ui->modif_ajout_enseignant->text());
                ui->typeEns->setCurrentText(ui->modif_ajout_type->currentText());
                ui->pushButton_11->click();
                ui->pushButton_12->click();
            }else{
                QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
                database.setDatabaseName(databaseLocation);
                if(!database.open()){
                    QMessageBox::warning(this,"erreur","base de données non connectée");
                    qDebug()<<database.lastError();
                }else{
                    QSqlQuery q;
                    q.prepare("insert into t_enseignement values (null,:type,:matiere)");
                    q.bindValue(":type",ui->modif_ajout_type->currentText());
                    q.bindValue(":matiere",ui->modif_ajout_matiere->text());
                    if(!q.exec()){
                        qDebug()<<q.lastError();
                        qDebug()<<q.lastQuery();
                    }else{
                        QMessageBox::information(this,"Enseignement","Enseignement Ajouté");
                        ui->stack_ens->setCurrentWidget(ui->stack_table_enseignement);
                    }
                }
            }
        }
    }else{//--------------------------------------------------------------------faire une modification
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseLocation);
        if(!database.open()){
            QMessageBox::warning(this,"erreur","base de données non connectée");
            qDebug()<<database.lastError();
        }else{
            QSqlQuery q;
            //----------------------Modifier la table 'l_etudiant' avec----------------------------------
            q.prepare("update t_absent set abs_type_ens='"+ui->modif_ajout_type->currentText()+"' where abs_numero_ens="+enseignementSelect+"");
            if(!q.exec()){
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }
            //------------------------------------------------------------------------------------------
            q.prepare("update t_enseignement set ens_type='"+ui->modif_ajout_type->currentText()+"',ens_nom='"+ui->modif_ajout_matiere->text()+"' where ens_numero="+enseignementSelect+"");
            if(!q.exec()){
                qDebug()<<q.lastError();
                qDebug()<<q.lastQuery();
            }else{
                QMessageBox::information(this,"Enseignement","Enseignement Modifié");
                ui->stack_ens->setCurrentWidget(ui->stack_table_enseignement);
                enseignementSelect="";
            }
        }
    }
    ui->pushButton_19->click();
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
}

//---------------------ANNULER L'AJOUT D'UN NOUVEL ENSEIGNEMENT---------------------
void MainWindow::on_pushButton_12_clicked()
{
    ui->stack_ens->setCurrentWidget(ui->stack_table_enseignement);
    ui->pushButton_19->click();
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
}


void MainWindow::on_modif_ajout_fichePres_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    if(ui->modif_ajout_fichePres->isChecked()){
        ui->modif_ajout_niveau->setEnabled(true);
        ui->modif_ajout_parcours->setEnabled(true);
        ui->dateEdit->setEnabled(true);
    }else{
        ui->modif_ajout_niveau->setEnabled(false);
        ui->modif_ajout_parcours->setEnabled(false);
        ui->dateEdit->setEnabled(false);
    }
}

//-----------------------------------cliquer dans le tableau 'liste d'enseignement'---------------------------------------
void MainWindow::on_enseignement_tableVW_clicked(const QModelIndex &index)
{
    ui->enseignement_tableVW->selectRow(index.row());
    ui->pushButton_21->setEnabled(true);
    ui->pushButton_22->setEnabled(true);
    enseignementSelect=ui->enseignement_tableVW->model()->itemData(index.siblingAtColumn(0))[0].toString();
    enseignementSelectMatiere=ui->enseignement_tableVW->model()->itemData(index.siblingAtColumn(1))[0].toString();
    enseignementSelectType=ui->enseignement_tableVW->model()->itemData(index.siblingAtColumn(2))[0].toString();
}

//---------------------------------modifier un enseignement------------------------------------------------------
void MainWindow::on_pushButton_21_clicked()
{
    ajout=false;
    ui->stack_ens->setCurrentWidget(ui->stack_ajouter_modifier_enseignement);
    ui->modif_ajout_fichePres->setEnabled(false);
    ui->modif_ajout_type->setCurrentText(enseignementSelectType);
    ui->modif_ajout_matiere->setText(enseignementSelectMatiere);
}

//-------------------------------------SUPPRIMER UN ENSEIGNEMENT-------------------------------------------------
void MainWindow::on_pushButton_22_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(!database.open()){
        QMessageBox::warning(this,"erreur","base de données non connectée");
        qDebug()<<database.lastError();
    }else{
        QSqlQuery q;
        //----------------------Modifier la table 'l_etudiant' avec----------------------------------
        q.prepare("delete from t_absent where abs_numero_ens="+enseignementSelect+"");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }
        //------------------------------------------------------------------------------------------
        q.prepare("delete from t_enseignement where ens_numero="+enseignementSelect+"");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
            QMessageBox::information(this,"Enseignement","Enseignement Supprimée");
            enseignementSelect="";
        }
    }
    ui->pushButton_19->click();
    ui->pushButton_21->setEnabled(false);
    ui->pushButton_22->setEnabled(false);
}

//-------------------------------------ACTUALISER LA TABLE DES FICHE DE PRESENCES-----------------------------------------------------
void MainWindow::actualize_table_consult_fichePres(){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQuery q;
        q.prepare("select t_absent.abs_absent as ABSENT, t_etudiant.et_numero, t_etudiant.et_nom||' '||t_etudiant.et_prenom as 'Nom & Prenom', t_absent.abs_numero_ens as 'Enseignement N°', t_etudiant.et_niveau as Niveau,t_etudiant.et_parcours as Parcours\
                  from t_absent left join t_etudiant on t_etudiant.et_numero=t_absent.abs_numero \
                  where t_absent.abs_date='"+ui->consult_fichePres_date->currentText()+"' \
                  and t_absent.abs_niveau='"+ui->consult_fichePres_niveau->currentText()+"' \
                  and t_etudiant.et_parcours='"+ui->consult_fichePres_parcours->currentText()+"'");
        if(!q.exec()){
            qDebug()<<q.lastError();
            qDebug()<<q.lastQuery();
        }else{
        QSqlQueryModel *qm =new QSqlQueryModel(this);
        qm->setQuery(q.lastQuery());
        ui->consult_fichePres_tableau->setModel(qm);
        ui->consult_fichePres_tableau->setColumnWidth(2,500);
        ui->consult_fichePres_tableau->setColumnWidth(3,250);
        }
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}
//-------------------------------------ACTUALISER LA TABLE DES FICHE DE PRESENCES-----------------------------------------------------

void MainWindow::on_consult_fichePres_date_currentTextChanged(const QString &arg1)
{
    ui->consult_fichePres_date->setCurrentText(arg1);
    actualize_table_consult_fichePres();
}

//-------------------------------------ACTUALISER LE COMBOBOX DE DATE DANS L'ONGLET 'CONSULTER LA FICHE DE PREENCE'-----------------------------------------------------
void MainWindow::actualize_comboBox_date_consult_fichePres(){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(databaseLocation);
    if(database.open()){
        QSqlQueryModel *qm = new QSqlQueryModel(this);
        QSqlQuery q;
        q.prepare("select abs_date from t_absent where abs_niveau='"+ui->consult_fichePres_niveau->currentText()+"' group by abs_date");
        qm->setQuery(q.lastQuery());
        ui->consult_fichePres_date->setModel(qm);
    }else{
        QMessageBox::warning(this,"error"," base de données non connecté");
        qDebug()<<database.lastError();
    }
}
//-------------------------------------ACTUALISER LE COMBOBOX DE DATE DANS L'ONGLET 'CONSULTER LA FICHE DE PREENCE'-----------------------------------------------------
void MainWindow::on_pushButton_6_clicked()
{
    actualize_table_consult_fichePres();
}
//-------------------------------------ACTUALISER LE COMBOBOX DE DATE DANS L'ONGLET 'CONSULTER LA FICHE DE PREENCE'-----------------------------------------------------
void MainWindow::on_consult_fichePres_parcours_currentTextChanged(const QString &arg1)
{
    ui->consult_fichePres_parcours->setCurrentText(arg1);
    actualize_comboBox_date_consult_fichePres();
}
//-------------------------------------ACTUALISER LE COMBOBOX DE DATE DANS L'ONGLET 'CONSULTER LA FICHE DE PREENCE'-----------------------------------------------------
void MainWindow::on_consult_fichePres_niveau_currentTextChanged(const QString &arg1)
{
    ui->consult_fichePres_niveau->setCurrentText(arg1);
    actualize_comboBox_date_consult_fichePres();
}



void MainWindow::on_enseignement_liste_acceuil_clicked()
{
    ui->menu->setCurrentIndex(6);
    QMessageBox::information(this,"Consulter la liste","Appuiez sur 'Actualiser' pour actualiser");
}


void MainWindow::on_enseignement_ajout_acceuil_clicked()
{
    ui->menu->setCurrentIndex(6);
    QMessageBox::information(this,"Ajouter un enseignement","1°Appuiez sur 'Ajouter'\n2°Dans la nouvelle page remplir les champs\n3°Appuiez sur 'OK' pou confirmer ou sur 'Annuler' pour annuler l'ajout");
}


void MainWindow::on_enseignement_modif_acceuil_clicked()
{
    ui->menu->setCurrentIndex(6);
    QMessageBox::information(this,"Modifier un enseignement","1°Selectionner une enseignement dans le tableau\n2°Appuiez sur 'Modifier'\n3°Dans la nouvelle page effectuer la modification\n4°Appuiez sur 'OK' pou confirmer ou sur 'Annuler' pour annuler la modification");
}


void MainWindow::on_enseignement_suppr_acceuil_clicked()
{
    ui->menu->setCurrentIndex(6);
    QMessageBox::information(this,"Supprimer un enseignement","1°Selectionner une enseignement dans le tableau\n2°Appuiez sur 'Supprimer'");
}


