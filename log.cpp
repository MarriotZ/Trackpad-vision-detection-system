#include "log.h"
#include "ui_log.h"
#include<QMessageBox>
#include<QSettings>
#include<QDebug>

Log::Log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
    setting=new QSettings("HKEY_CURRENT_USER\\Software\\Lenovo\\Test",QSettings::NativeFormat);
    if(setting->value("pw1").toString().length()==0){
        setting->setValue("pw1","111111");
        setting->setValue("pw2","222222");
    }
    pw1=setting->value("pw1").toString();
    pw2=setting->value("pw2").toString();
    ui->stackedWidget->setCurrentIndex(0);
    QRegExp regx("^[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,this);
    ui->lineEdit_NewPw1->setValidator(validator);
    ui->lineEdit_NewPw2->setValidator(validator);
    ui->lineEdit_NewPw1->setMaxLength(10);
    ui->lineEdit_NewPw2->setMaxLength(10);

}

Log::~Log()
{
    delete ui;
}

void Log::on_btn_Log_clicked()
{
    int x;
    x=ui->comBox_UserType->currentIndex();
    if(x==0){
        if(ui->lineEdit_Password->text()==pw1){
            emit(login1());
            qDebug()<<QString::fromLocal8Bit("操作员登陆");
            this->close();
        }
        else
        {
            QMessageBox::warning(this,"error","Wrong Password");
            ui->lineEdit_Password->clear();
        }
    }
    else{
        if(ui->lineEdit_Password->text()==pw2){
            emit(login2());
            qDebug()<<QString::fromLocal8Bit("管理员登陆");
            this->close();
        }
        else
        {
            QMessageBox::warning(this,"error","Wrong Password");
            ui->lineEdit_Password->clear();
        }
    }


}

void Log::on_btn_ChangePassword_clicked()
{
    int x;
    x=ui->comBox_UserType->currentIndex();
    if(x==0){
        if(ui->lineEdit_Password->text()==pw1){
            ui->lineEdit_Password->clear();
            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            QMessageBox::warning(this,"error","Wrong Password");
            ui->lineEdit_Password->clear();
        }
    }
    else{
        if(ui->lineEdit_Password->text()==pw2){
            ui->lineEdit_Password->clear();
            ui->stackedWidget->setCurrentIndex(1);
        }
        else
        {
            QMessageBox::warning(this,"error","Wrong Password");
            ui->lineEdit_Password->clear();
        }
    }
}

void Log::on_btn_Exit_clicked()
{
    close();
}

void Log::on_btn_Yes_clicked()
{
    if(ui->lineEdit_NewPw1->text().length()<6){
        QMessageBox::warning(this,"error","The password length cannot be less than six digits");
        ui->lineEdit_NewPw1->clear();
        ui->lineEdit_NewPw2->clear();
    }
    else if(ui->lineEdit_NewPw1->text()==ui->lineEdit_NewPw2->text()){
        int x;
        x=ui->comBox_UserType->currentIndex();
        if(x==0){
            setting->setValue("pw1",ui->lineEdit_NewPw1->text());
            pw1=ui->lineEdit_NewPw1->text();
        }
        else{
            setting->setValue("pw2",ui->lineEdit_NewPw1->text());
            pw2=ui->lineEdit_NewPw1->text();
        }
        ui->lineEdit_NewPw1->clear();
        ui->lineEdit_NewPw2->clear();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else{
        QMessageBox::warning(this,"error","The passwords entered twice are inconsistent");
        ui->lineEdit_NewPw1->clear();
        ui->lineEdit_NewPw2->clear();
    }
}

void Log::on_btn_No_clicked()
{
    ui->lineEdit_NewPw1->clear();
    ui->lineEdit_NewPw2->clear();
    ui->stackedWidget->setCurrentIndex(0);
}


