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
    m_netAccess = new NetAccess("http://www.csu.edu.cn",this);
    setIcon(QIcon(":/res/icon.ico"));
    QMenu *menu = new QMenu();
    menu->addAction(tr("show"),m_appUi,SLOT(show()));
    menu->addAction(tr("exit"),this,SLOT(quitApp()));
    setContextMenu(menu);
    connect(m_appUi,SIGNAL(quitClicked()),this,SLOT(quitApp()));
    connect(m_appUi,SIGNAL(loginClicked(QString,QString)),m_netAccess,SLOT(login(QString,QString)));
    connect(m_appUi,SIGNAL(logoutClicked()),m_netAccess,SLOT(logout()));
    connect(m_netAccess,SIGNAL(replyLogin(Data)),m_appUi,SLOT(showReplyInfo(Data)));
    connect(m_netAccess,SIGNAL(replyLogout(Data)),m_appUi,SLOT(showReplyInfo(Data)));
    m_appUi->show();
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
