#include "netaccess.h"
#include <QUrl>
#include <QRegExp>
#include <QScriptEngine>
#include <QFile>
#include <QJsonDocument>
#include <QVariant>
#include <QMessageBox>
#include <stdlib.h>
#include <QDebug>


NetAccess::NetAccess(QObject *parent) :
    QObject(parent),m_logState(false),m_hasCookie(false)
{
    m_netaccess = new QNetworkAccessManager(this);//初始化连接器
    m_cookiejar = new QNetworkCookieJar(this);//初始化cookiejar
    m_loop = new QEventLoop(this);//初始化事件循环
    connect(m_netaccess,SIGNAL(finished(QNetworkReply*)),this,SLOT(ServerReplyHandler(QNetworkReply*)));
}

bool NetAccess::getLogState(){
    return m_logState;
}

void NetAccess::login(QString userId,QString userPass){
    getInitArgument();
    m_request.setUrl(QUrl("http://61.137.86.87:8080/portalNat444/AccessServices/login"));
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=utf-8");
    m_request.setRawHeader(QByteArray("Referer"),QByteArray("http://61.137.86.87:8080/portalNat444/index.jsp"));
    QString postBody = QString("accountID=%1\%40zndx.inter&password=%2&brasAddress=%3&userIntranetAddress=%4").arg(userId)
            .arg(encodePasswd(userPass)).arg(m_braAddress).arg(m_userIntranetAddress);
    m_netaccess->post(m_request,postBody.toUtf8());
}

void NetAccess::logout(){
    m_request.setUrl(QUrl("http://61.137.86.87:8080/portalNat444/AccessServices/logout?"));
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded;charset=utf-8");
    m_request.setRawHeader(QByteArray("Referer"),QByteArray("http://61.137.86.87:8080/portalNat444/main2.jsp"));
    QString postBody = QString("brasAddress=%1&userIntranetAddress=%2").arg(m_braAddress).arg(m_userIntranetAddress);
    m_netaccess->post(m_request,postBody.toUtf8());
    m_loop->exec();
}

void NetAccess::ServerReplyHandler(QNetworkReply *reply){//处理来自服务器的响应
    m_loop->exit();                     //结束事件循环
    if(reply->error()!=QNetworkReply::NoError){ //如果响应有误则退出    
        throw reply->errorString();
    }
    if(m_hasCookie==false&&m_logState==false){   //无cookie以及无登陆则为初始化响应
        initGetReply(reply);
        return ;
    }
    if(m_hasCookie==true&&m_logState==false){    //有cookie以及无登陆则为登陆响应
        loginReply(reply);
        return;
    }
    if(m_hasCookie==true&&m_logState==true){     //有cookie以及有登陆则为注销响应
        logoutReply(reply);
        return;
    }
}

void NetAccess::getInitArgument(){
  try{
    m_request.setUrl(QUrl("http://www.csu.edu.cn"));//初始化连接请求
    m_netaccess->get(m_request);
    m_loop->exec();
    m_netaccess->get(m_request);
    m_loop->exec();
    m_netaccess->get(m_request);
    m_loop->exec();
   }
    catch(QString err){
      QMessageBox::critical(0,"Error",err,QMessageBox::Ok);
    }
}

void NetAccess::loginReply(QNetworkReply *reply){//登陆响应处理函数
    QJsonDocument jsondoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonData= jsondoc.object();
    Data info;
    info.action = Data::LOGIN;
    info.resultCode = jsonData.take("resultCode").toVariant().toInt();
    if(info.resultCode==0||info.resultCode==11){
       info.usedflow = jsonData.take("usedflow").toString();
       info.totalflow = jsonData.take("totalflow").toString();
       info.surplusmoney = jsonData.take("surplusmoney").toString();
       emit replyLogin(info);
       m_logState = true;
       return;
    }
    info.resultDescribe = jsonData.take("resultDescribe").toString();
    emit replyLogin(info);
}

void NetAccess::initGetReply(QNetworkReply *reply){
    if(reply->hasRawHeader(QByteArray("Set-Cookie"))){//如果包含该字段则设置cookiejar
        QList<QNetworkCookie> cookie= QNetworkCookie::parseCookies(reply->rawHeader(QByteArray("Set-Cookie")));
        m_cookiejar->setCookiesFromUrl(cookie,QUrl(QByteArray("http://61.137.86.87:8080")));
        m_netaccess->setCookieJar(m_cookiejar);
    }
    m_location = QString(reply->rawHeader(QByteArray("Location")));
    m_request.setUrl(QUrl(m_location));
    QByteArray body = reply->readAll(); //获取响应体的内容
    if(body.contains("<html>")){    //如果带有html标签则开始寻找目标
        QRegExp pattern;
        int indexS(0),indexF(0);                //包含信息字串的起始位置和终止位置
        indexS = body.lastIndexOf("input");     //从末尾开始搜索input域内的字串
        if (indexS==-1)
            return;
        for(int i=indexS;;i++){
            if(body[i]=='/'){
                indexF = i-1;
                break;
            }
        }
        pattern.setPattern("([0-9a-f]{8})");        //设置查询braAddress的正则式
        pattern.indexIn(QString(body.mid(indexS,indexF-indexS)));
        m_braAddress = pattern.capturedTexts().at(0);
        indexS = body.lastIndexOf("input",--indexS);
        if (indexS==-1)
            return;
        for(int i=indexS;;i++){
         if(body[i]=='/'){
             indexF = i-1;
             break;
            }
        }
        pattern.setPattern("([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3})");//设置查询ip的正则式
        pattern.indexIn(QString(body.mid(indexS,indexF-indexS)));
        m_userIntranetAddress = pattern.capturedTexts().at(0);
        m_hasCookie = true;
  }
}

void NetAccess::logoutReply(QNetworkReply *reply){//注销响应处理函数
    QJsonDocument jsondoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonData= jsondoc.object();
    Data info;
    info.action = Data::LOGOUT;
    info.resultCode = jsonData.take("resultCode").toVariant().toInt();
    if(info.resultCode==0||info.resultCode==3){
       m_logState = false;
       emit replyLogout(info);
       return;
    }
    info.resultDescribe = jsonData.take("resultDescribe").toString();
    emit replyLogin(info);
}

QString NetAccess::encodePasswd(QString passwd){//对密码进行RSA公钥加密的函数
    QScriptEngine engine;                       //脚本解析引擎
    QScriptValueList args;                      //脚本执行参数列表
    args<<passwd;                               //以下先将传入的密码原文进行URI编码
    QScriptValue encodePasswd = engine.evaluate("encodeURIComponent").call(QScriptValue(),args);
    args.clear();
    QFile jsFile(":/res/security.js");
    jsFile.open(QIODevice::ReadOnly);
    //从此处开始，开始解析脚本文件
    QString jsBody = QString(jsFile.readAll());
    args<<"10001"<<""<<"a8a02b821d52d3d0ca90620c78474b78435423be99da83cc190ab5cb5b9b922a4c8ba6b251e78429757cf11cde119e1eacff46fa3bf3b43ef68ceb29897b7aa6b5b1359fef6f35f32b748dc109fd3d09f3443a2cc3b73e99579f3d0fe6a96ccf6a48bc40056a6cac327d309b93b1d61d6f6e8f4a42fc9540f34f1c4a2e053445";
    QScriptValue RSAUtil = engine.evaluate(jsBody);             //获取RSAUtil对象
    //利用获取的对象获取公钥
    QScriptValue pubkey = RSAUtil.property("getKeyPair").call(QScriptValue(),args);
    args.clear();
    args<<pubkey<<encodePasswd;
    //将公钥与编码过的密码进行RSA公钥加密，然后返回
    QString result = RSAUtil.property("encryptedString").call(QScriptValue(),args).toString();
    return result;
}
