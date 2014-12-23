#ifndef INPUTGEN_H
#define INPUTGEN_H

#include <QDialog>
#include <QWidget>
#include <vector>

using namespace std;

class QLineEdit;
class QRadioButton;
class QCheckBox;
class QTextEdit;
class QButtonGroup;
class InputView;

class InputGen : public QDialog{
    
    Q_OBJECT
    
    public:
        InputGen(QWidget *parent = 0);        
    
    private slots:
        void startClicked();
        void exitClicked();
        void saveClicked();
        void check();
        void checkFile();
        
    private:
        InputView *inputView;
        QLineEdit *core1;
        QLineEdit *core2;
        QLineEdit *core3;        
        QLineEdit *shell1;
        QLineEdit *shell2;
        QLineEdit *shell3;        
        QLineEdit *randMin;
        QLineEdit *randMax;
        QLineEdit *iterField;
        QLineEdit *fileField;
        QButtonGroup *methodGroup;
        QRadioButton *etchButton;
        QRadioButton *growButton;
        QCheckBox *displayBox;
        QCheckBox *verboseBox;
        QTextEdit *statusText;
        QPushButton *startButton;
        QPushButton *saveButton;
        QPushButton *quitButton;
        vector< vector< vector< int > > > space;
        void drawPoints(int);
};

#endif
