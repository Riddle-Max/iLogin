#ifndef DATA_H
#define DATA_H
#include<QString>

struct Data{
    enum Action{LOGIN,LOGOUT};
    Action action;
    int resultCode;
    QString totalflow;
    QString usedflow;
    QString surplusmoney;
    QString resultDescribe;
    Data():resultCode(-1){}
};

#endif // DATA_H
