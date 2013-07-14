#include "appui.h"
#include "ui_appui.h"
#include <QMouseEvent>
#include <QSettings>
#include <QMessageBox>
#include <QVariant>
#include <QPainter>
#include <QMenu>

AppUI::AppUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppUI)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    m_config = new QSettings(QSettings::UserScope,qApp->organizationName(),qApp->applicationName(),this);
    m_bgpixmap.load(":/res/bg.jpg");
    setWindowIcon(QIcon(":/res/icon.ico"));
    getConfig();
    QMenu *menu = new QMenu(this);
    menu->addAction(tr("关于..."),this,SLOT(aboutThis()));                //添加的工具栏菜单
    ui->b_menu->setMenu(menu);
    connect(ui->b_login,SIGNAL(clicked()),this,SLOT(clickLogin()));
    connect(ui->b_logout,SIGNAL(clicked()),this,SIGNAL(logoutClicked()));
    connect(ui->b_trayicon,SIGNAL(clicked()),this,SLOT(hide()));
    connect(ui->b_quit,SIGNAL(clicked()),this,SIGNAL(quitClicked()));
    connect(ui->remeberPasswd,SIGNAL(clicked()),this,SLOT(checkRememberPasswd()));
}

AppUI::~AppUI()
{
    delete ui;
}

void AppUI::showReplyInfo(Data info){
    if(info.action==Data::LOGIN){//判断信息针对动作的类型
        switch(info.resultCode){
        case 0:
        case 11:
            //QMessageBox::information(this,tr("登陆"),tr("登陆成功"),QMessageBox::Ok);
            ui->totalflow->setText(info.totalflow);             //设置总流量
            ui->usedflow->setText(info.usedflow);               //设置使用流量
            ui->surplusmoney->setText(info.surplusmoney);       //设置余额
            ui->stackedWidget->setCurrentIndex(1);   //用于改变StackWidget当前界面
            break;
        default:
            QMessageBox::warning(this,tr("登陆"),info.resultDescribe.length()!=0?info.resultDescribe:"其他原因拒绝认证",QMessageBox::Ok);
            break;
        }
    }
    if(info.action==Data::LOGOUT){
        switch(info.resultCode){
        case 0:
        case 3:
            QMessageBox::information(this,tr("注销"),tr("注销成功"),QMessageBox::Ok);
            if(!ui->remeberPasswd->isChecked()){
                ui->lineEdit_a->clear();
                ui->lineEdit_p->clear();
            }
            ui->stackedWidget->setCurrentIndex(0);
            qApp->quit();
            break;
        default:
            QMessageBox::warning(this,tr("注销"),info.resultDescribe,QMessageBox::Ok);
            break;
        }
    }
}

void AppUI::mouseMoveEvent(QMouseEvent *e)
{
    this->move(e->globalPos() - m_pointStart);
}

void AppUI::mousePressEvent(QMouseEvent *e)
{
    m_pointPress = e->globalPos();
    m_pointStart = m_pointPress - this->pos();

}

void AppUI::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(m_bgpixmap.rect(),m_bgpixmap);
}

void AppUI::clickLogin(){
    if(ui->lineEdit_a->text().length()==0||ui->lineEdit_p->text().length()==0){
        QMessageBox::warning(this,tr("输入不能为空"),tr("账号或者密码不能为空！"),QMessageBox::Ok);
        return;
    }
    emit loginClicked(ui->lineEdit_a->text(),ui->lineEdit_p->text());
}
void AppUI::checkRememberPasswd(){
    if(ui->remeberPasswd->isChecked()){
        setConfig();
    }
     m_config->setValue("checked",ui->remeberPasswd->isChecked());
}

void AppUI::aboutThis(){
    QMessageBox::about(this,tr("about this software"),tr("iLogin v1.0<br>Based on Qt 5.1,under GPLv3 licence"));
}

void AppUI::getConfig(){
    bool checked = m_config->value("checked",false).toBool();       //获取记住密码的值
    ui->remeberPasswd->setChecked(checked);
    if(checked==true){                                              //如果为真就填充记住的账号密码
        QString account = m_config->value("account").toString();
        QString passwd = m_config->value("passwd").toString();
        ui->lineEdit_a->setText(account);
        ui->lineEdit_p->setText(passwd);
    }
}

void AppUI::setConfig(){
    m_config->setValue("account",ui->lineEdit_a->text());
    m_config->setValue("passwd",ui->lineEdit_p->text());
}
