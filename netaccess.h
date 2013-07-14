#ifndef NETACCESS_H
#define NETACCESS_H

#include <QObject>
#include <QtNetwork>
#include <QEventLoop>
#include "Data.h"

//网络通信类，负责将用户的密码以及账号信息发送到服务器，并接收和解析返回结果

class NetAccess : public QObject
{
    Q_OBJECT

public:
    explicit NetAccess(const QString &,QObject *parent = 0);
    bool getLogState();  
signals:
    void replyLogin(Data replyInfo); //获取响应代码
    void replyLogout(Data replyInfo);
public slots:
    void login(QString userId,QString userPass);        //登录信息槽
    void logout();                                      //注销登陆槽
private slots:
    void ServerReplyHandler(QNetworkReply*);      //初始化响应处理槽
private:
    void loginReply(QNetworkReply *);                        //登陆响应
    void initGetReply(QNetworkReply *);                      //初始化时Get的响应，获取Cookie
    void logoutReply(QNetworkReply *);                       //注销响应
    QString encodePasswd(QString);
    QNetworkAccessManager *m_netaccess;       //网络连接管理器，用于与服务器建立连接，发出请求等
    QNetworkRequest m_request;                //网络请求
    QNetworkCookieJar *m_cookiejar;           //连接用的cookiejar
    QEventLoop *m_loop;                       //自定义事件循环，使管理器可以等待响应事件到来
    QString m_location;                       //用于给请求进行URL设置
    QString m_userIntranetAddress;            //登陆用参数,代表IP
    QString m_braAddress;                     //登陆用参数，代表设备地址
    bool m_logState;                          //登录状态标记
    bool m_hasCookie;                         //登陆的前提条件完成标记
};

#endif // NETACCESS_H
