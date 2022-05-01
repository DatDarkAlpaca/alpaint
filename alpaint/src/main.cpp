#include "pch.h"
#include "alpaint/Alpaint.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Alpaint w;
    w.show();

    return app.exec();
}
