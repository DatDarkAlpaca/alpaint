#include "pch.h"
#include "alpaint/Alpaint.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    alp::Alpaint w;
    w.show();

    return app.exec();
}
