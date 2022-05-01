#include "pch.h"
#include "alpaint/Alpaint.h"
#include "pallete/DarkPallete.h"

int main(int argc, char *argv[])
{
    DarkPallete pallete;

    QApplication app(argc, argv);

    Alpaint w;
    w.show();

    app.setPalette(pallete());
    return app.exec();
}
