#ifndef APPUI_H
#define APPUI_H

#include <QDialog>
#include "Data.h"
#include <QSystemTrayIcon>

namespace Ui {
class AppUI;
}

class QMouseEvent;
class QSettings;

class AppUI : public QDialog
{
    Q_OBJECT
    
public:
    explicit AppUI(QWidget *parent = 0);
    ~AppUI();
signals:
    void loginClicked(QString userId,QString userPass);         //登陆信号
    void logoutClicked();                                       //注销信号
    void quitClicked();                                         //退出信号
public slots:
    void showReplyInfo(Data info);
    void show(QSystemTrayIcon::ActivationReason);
private slots:
    void clickLogin();
    void checkRememberPasswd();
    void aboutThis();
protected:
    void mouseMoveEvent(QMouseEvent *);                         //重载UI拖动函数
    void mousePressEvent(QMouseEvent *);
    //void paintEvent(QPaintEvent *);                             //重载UI背景绘制函数
private:
    void getConfig();       //获取配置信息
    void setConfig();       //存储配置信息  
    Ui::AppUI *ui;          //界面布局
    QPoint m_pointStart;    //起点坐标
    QPoint m_pointPress;    //终点坐标
    QSettings *m_config;    //配置信息
    //QPixmap m_bgpixmap;     //界面背景
};

#endif // APPUI_H
