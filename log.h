#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include<QSettings>

namespace Ui {
class Log;
}

class Log : public QWidget
{
    Q_OBJECT

signals:
    void login1();
    void login2();

public:
    explicit Log(QWidget *parent = nullptr);
    ~Log();

private slots:
    void on_btn_Log_clicked();

    void on_btn_ChangePassword_clicked();

    void on_btn_Exit_clicked();

    void on_btn_No_clicked();

    void on_btn_Yes_clicked();

private:
    Ui::Log *ui;
    QString pw1;
    QString pw2;
    QSettings *setting;
};

#endif // LOG_H
