#include "pch.h"
#include "ToolHandler.h"
#include "alpaint/Alpaint.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    alp::ToolHandler::initializeTools();

    alp::Alpaint w;
    w.show();

    return app.exec();
}
