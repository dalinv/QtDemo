#include "setdesennumdlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString strPhone="18836114600";
    SetDesenNumDlg w(strPhone);
    w.show();
    return a.exec();
}
