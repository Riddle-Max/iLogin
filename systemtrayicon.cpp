#include "systemtrayicon.h"
#include "appui.h"
#include "netaccess.h"
#include <QMenu>
#include <QApplication>
#include <QIcon>

SystemTrayIcon::SystemTrayIcon(QObject *parent):
    QSystemTrayIcon(parent)
{
    m_appUi = new AppUI();
    m_netAccess = new NetAccess(this);
    setIcon(QIcon(":/res/icon.png"));
    connect(m_appUi,SIGNAL(quitClicked()),this,SLOT(quitApp()));
    connect(m_appUi,SIGNAL(loginClicked(QString,QString)),m_netAccess,SLOT(login(QString,QString)));
    connect(m_appUi,SIGNAL(logoutClicked()),m_netAccess,SLOT(logout()));
    connect(m_netAccess,SIGNAL(replyLogin(Data)),m_appUi,SLOT(showReplyInfo(Data)));
    connect(m_netAccess,SIGNAL(replyLogout(Data)),m_appUi,SLOT(showReplyInfo(Data)));
    connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),m_appUi,SLOT(show(QSystemTrayIcon::ActivationReason)));
    m_appUi->QDialog::show();
}

SystemTrayIcon::~SystemTrayIcon(){
    delete m_appUi;
}

void SystemTrayIcon::quitApp(){
    if(m_netAccess->getLogState()==true){
        m_netAccess->logout();
    }
    qApp->quit();
}
