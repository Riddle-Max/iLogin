#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QSystemTrayIcon>

class NetAccess;
class AppUI;

//系统任务图标类，负责将应用程序以图标形式显示在任务栏

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit SystemTrayIcon(QObject *parent=0);
    ~SystemTrayIcon();
signals:

private slots:
    void quitApp();
private:
    NetAccess *m_netAccess;
    AppUI *m_appUi;
};

#endif // SYSTEMTRAYICON_H
