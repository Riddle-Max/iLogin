#ifndef QSINGLEAPPLICATION_H
#define QSINGLEAPPLICATION_H

#include <QApplication>

//应用程序监视类，使应用程序只能启动一个实例

class QSingleApplication : public QApplication
{
public:
    QSingleApplication(int & argc, char ** argv );
};

#endif // QSINGLEAPPLICATION_H
