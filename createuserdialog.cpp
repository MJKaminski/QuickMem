/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#include "createuserdialog.h"
#include "ui_createuserdialog.h"
#include <QPushButton>
#include <QMessageBox>

/** CreateUserDialog potrzebuje wskaznika na liste kursow oraz zmienna do zapisu nazwy uzytkownika*/
CreateUserDialog::CreateUserDialog(QWidget *parent, QString *s, QStringList* cList) :
    QDialog(parent),
    ui_(new Ui::CreateUserDialog), userName_(s), courseList_(cList)
{
    ui_->setupUi(this);
    setWindowTitle("Dodaj użytkownika");

    for(auto i = (*cList).begin(); i!=(*cList).end(); ++i) //tworzenie listy z opcja zaznaczania pozycji
    {
        QListWidgetItem* item = new QListWidgetItem(*i, ui_->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

CreateUserDialog::~CreateUserDialog()
{
    delete ui_;
}

/** Zamyka dialog z sygnalem anuluj*/
void CreateUserDialog::on_rejectButton_clicked()
{
    reject();
}

/** Zamyka dialog z sygnalem akceptacji
 * nazwa nowego uzytkownika znajduje sie po adresem userName*/
void CreateUserDialog::on_acceptButton_clicked()
{
    if(ui_->userNameEdit->text().trimmed() == "")
        QMessageBox::critical(this,"Błąd","Nazwa użytkownika nie może być pusta!");
    else
    {
        *userName_ = ui_->userNameEdit->text();
        auto iter = courseList_ -> begin();
        for(int i = 0; i < ui_->listWidget->count(); ++i)
        {
            if(ui_->listWidget->item(i)->checkState() == Qt::Unchecked)
                iter = courseList_->erase(iter);
            else
                ++iter;
        }
        accept();
    }
}
