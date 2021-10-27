#include "checkfilemd5.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CheckFileMd5 w;
    w.show();
    return a.exec();
}
