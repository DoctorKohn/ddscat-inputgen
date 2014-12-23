#include <QApplication>
#include <QDesktopWidget>

#include "inputgen.h"

int main(int argc, char *argv[]){
    
    QApplication app(argc, argv);
    InputGen gen;
    gen.show();
    return app.exec();
    
}
