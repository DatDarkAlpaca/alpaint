#include "pch.h"
#include "ToolHandler.h"
#include "alpaint/Alpaint.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationDomain("datalpaca.com");
    QCoreApplication::setOrganizationName("DatAlpaca");
    QCoreApplication::setApplicationName("Alpaint");

    alp::ToolHandler::initializeTools();

    alp::Alpaint w;
    w.show();

    return app.exec();
}
