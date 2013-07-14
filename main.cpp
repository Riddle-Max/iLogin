#include "qsingleapplication.h"
#include "systemtrayicon.h"

int main(int argc, char *argv[])
{
    QSingleApplication a(argc, argv);
    a.setApplicationName("iClient");
    a.setApplicationVersion("v1.0");
    a.setOrganizationName("Central South University");
    SystemTrayIcon app;
    app.show();
    return a.exec();
}
