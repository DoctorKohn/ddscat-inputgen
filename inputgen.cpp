#include <QtGui>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "inputgen.h"
#include "inputview.h"

InputGen::InputGen(QWidget *parent) : QDialog(parent){
    // Instantiate all the stuff
    inputView = new InputView;
    QIntValidator *validator = new QIntValidator(0, 9999, this);
    QRegExp rx("^\\S.*$");
    QRegExpValidator *fileValidator = new QRegExpValidator(rx, this);
    core1 = new QLineEdit();
    core1->setValidator(validator);
    core2 = new QLineEdit();
    core2->setValidator(validator);
    core3 = new QLineEdit();
    core3->setValidator(validator);
    shell1 = new QLineEdit();
    shell1->setValidator(validator);
    shell2 = new QLineEdit();
    shell2->setValidator(validator);
    shell3 = new QLineEdit();
    shell3->setValidator(validator);
    randMin = new QLineEdit();
    randMin->setValidator(validator);
    randMax = new QLineEdit();
    randMax->setValidator(validator);
    iterField = new QLineEdit();
    iterField->setValidator(validator);
    fileField = new QLineEdit();
    fileField->setText(tr("shapeTest.dat"));
    fileField->blockSignals(true);
    fileField->setValidator(fileValidator);
    QLabel *coreLabel = new QLabel(tr("Core radii"));
    coreLabel->setToolTip(tr("X, Y, Z radius of core"));
    QLabel *shellLabel = new QLabel(tr("Shell radii"));
    QLabel *randLabel = new QLabel(tr("Random Min/Max"));
    QLabel *iterLabel = new QLabel(tr("Iterations"));
    QLabel *fileLabel = new QLabel(tr("File Name"));
    displayBox = new QCheckBox(tr("Display"));
    verboseBox = new QCheckBox(tr("Verbose"));
    etchButton = new QRadioButton(tr("Etch"));
    etchButton->setEnabled(false);
    growButton = new QRadioButton(tr("Grow"));
    growButton->setEnabled(false);
    growButton->setChecked(true);
    methodGroup = new QButtonGroup();
    methodGroup->addButton(etchButton);
    methodGroup->addButton(growButton);
    startButton = new QPushButton(tr("Start"));
    startButton->setEnabled(false);
    quitButton = new QPushButton(tr("Quit"));
    saveButton = new QPushButton(tr("Save"));
    saveButton->setEnabled(false);
    statusText = new QTextEdit(tr("Prepped and ready!"));
    statusText->setReadOnly(true);
    
    // Create the layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    
    // Add things to the layout
    mainLayout->addWidget(coreLabel, 0, 0);
    mainLayout->addWidget(core1, 0, 1);
    mainLayout->addWidget(core2, 0, 2);
    mainLayout->addWidget(core3, 0, 3);
    
    mainLayout->addWidget(shellLabel, 1, 0);
    mainLayout->addWidget(shell1, 1, 1);
    mainLayout->addWidget(shell2, 1, 2);
    mainLayout->addWidget(shell3, 1, 3);
    
    mainLayout->addWidget(randLabel, 2, 0);
    mainLayout->addWidget(randMin, 2, 1);
    mainLayout->addWidget(randMax, 2, 2);
    
    mainLayout->addWidget(iterLabel, 3, 0);
    mainLayout->addWidget(iterField, 3, 2);
    
    mainLayout->addWidget(etchButton, 4, 0);
    mainLayout->addWidget(growButton, 5, 0);
    mainLayout->addWidget(displayBox, 4, 2);
    mainLayout->addWidget(verboseBox, 5, 2);
    
    mainLayout->addWidget(fileLabel, 6, 0);
    mainLayout->addWidget(fileField, 6, 1, 1, 3);
    
    mainLayout->addWidget(startButton, 7, 1);
    mainLayout->addWidget(saveButton, 7, 2);
    mainLayout->addWidget(quitButton, 7, 3);
    
    mainLayout->addWidget(statusText, 8, 0, 1, 4);
    
    mainLayout->addWidget(inputView, 0, 4, 9, 1);
    
    setLayout(mainLayout);
    setWindowTitle(tr("DDSCAT Input Generator"));
    
    inputView->setVisible(false);
    
    // Create SIGNAL-SLOT connections
    connect(startButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(shell1, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(shell2, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(shell3, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(core1, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(core2, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(core3, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(randMin, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(randMax, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(iterField, SIGNAL(textEdited(const QString)), this, SLOT(check()));
    connect(methodGroup, SIGNAL(buttonClicked(int)), this, SLOT(check()));
    connect(fileField, SIGNAL(textEdited(const QString)), this, SLOT(checkFile()));
}
void InputGen::check(){
    
    // Check to see if all 3 core fields have valid input
    // If so, unlock the grow button
    if (core1->hasAcceptableInput()){
        if (core2->hasAcceptableInput()){
            if (core3->hasAcceptableInput()){
                growButton->setEnabled(true);
            }
            else growButton->setEnabled(false);
        }
        else growButton->setEnabled(false);
    }
    else growButton->setEnabled(false);
    
    // Check to see if all 3 shell fields have valid input
    // If so, unlock the etch button only if the grow button is unlocked
    if (shell1->hasAcceptableInput()){
        if (shell2->hasAcceptableInput()){
            if (shell3->hasAcceptableInput()){
                if (growButton->isEnabled()){                    
                    etchButton->setEnabled(true);
                }
                else etchButton->setEnabled(false);
            }
            else etchButton->setEnabled(false);
        }
        else etchButton->setEnabled(false);
    }
    else etchButton->setEnabled(false);
    
    // Check to see if iterations and min/max have valid input
    // Check to see if a method is selected AND enabled
    // If so, unlock the start button
    if (randMin->hasAcceptableInput()){
        if (randMax->hasAcceptableInput()){
            if (iterField->hasAcceptableInput()){
                if (methodGroup->checkedButton()->isEnabled()){
                    startButton->setEnabled(true);
                }
                else startButton->setEnabled(false);
            }
            else startButton->setEnabled(false);
        }
        else startButton->setEnabled(false);
    }
    else startButton->setEnabled(false);
}

void InputGen::checkFile(){
    
    // Check to see if file field is blank
    // If it is, lock the save button
    if (fileField->hasAcceptableInput()){
        saveButton->setEnabled(true);
    }
    else saveButton->setEnabled(false);
}
void InputGen::startClicked(){
    
    // Run the program!
    statusText->clear();
    inputView->clearSpheres();
    statusText->append("Starting...");
    bool etch = etchButton->isChecked();
    bool grow = growButton->isChecked();
    bool verbose = verboseBox->isChecked();
    float r1 [3] = {core1->text().toFloat(), core2->text().toFloat(),
        core3->text().toFloat()};
    float r2 [3] = {shell1->text().toFloat(), shell2->text().toFloat(),
        shell3->text().toFloat()};
    int r3 [2] = {randMin->text().toInt(), randMax->text().toInt()};
    int p = 1;
    if (etch){
        p = max(max(r2[0]+r3[1]*2, r2[1]+r3[1]*2), r2[2]+r3[1]*2);
    } else{
        p = max(max(r1[0]+r3[1]*2, r1[1]+r3[1]*2), r1[2]+r3[1]*2);
    }
    
        
    //int space [2*p][2*p][2*p];

    space.resize(2*p);
    float rA;
    float rB;
    statusText->append("Building the core...");
    for (int x = -p; x < p; x++){
        space[x+p].resize(2*p);
        for (int y = -p; y < p; y++){
            space[x+p][y+p].resize(2*p);
            for (int z = -p; z < p; z++){
                space[x+p][y+p][z+p] = 0;
                rA = pow(x/r1[0], 2) + pow(y/r1[1], 2)
                        + pow(z/r1[2], 2);
                //std::cout << rA;
               // std::cout << "\n";
                if (etch){
                    rB = ((pow(x/r2[0], 2)) + (pow(y/r2[1], 2))
                        + (pow(z/r2[2], 2)));
                    if (rB <= 1.0) space[x+p][y+p][z+p] = 1;
                }
                if (rA <= 1.0){
                    if (etch){
                        space[x+p][y+p][z+p] = 2;
                    } else {
                        space[x+p][y+p][z+p] = 1;
                    }
                }
            }
        }
    }
    // Debug
    /*for (int x = -p; x < p; x++){
        for (int y = -p; y < p; y++){
            for (int z = -p; z < p; z++){
               // cout << space[x+p][y+p][z+p];
            }
            //cout << endl;
        }
    }*/
    int num = 0;
    int mod = r3[1] - r3[0] + 1;
    
    statusText->append("Random section...");
    while (num < iterField->text().toInt()){
        int r31 = rand() % mod + r3[0];
        int r32 = rand() % mod + r3[0];
        int r33 = rand() % mod + r3[0];
        int a = rand() % (2*p-2*r31) + r31;
        int b = rand() % (2*p-2*r32) + r32;
        int c = rand() % (2*p-2*r33) + r33;
        //cout << r31;
        //cout << r32;
        //cout << r33 << endl;
        int test [6] = {space[a-r31][b][c],
                        space[a+r31][b][c],
                        space[a][b-r32][c],
                        space[a][b+r32][c],
                        space[a][b][c-r33],
                        space[a][b][c+r33]};
        bool zeros = false;
        bool ones = false;
        bool twos = false;
        for (int i = 0; i < 6; i++){
            if (test[i] == 2 && etch) twos = true;
            if (test[i] == 1) ones = true;
            if (test[i] == 0) zeros = true;
        }
        if (twos) continue;
        if (ones && zeros){
            num ++;
            if (verbose) statusText->append(QString::number(num));
            for (int i = -p; i < p; i++){
                for (int j = -p; j < p; j++){
                    for (int k = -p; k < p; k++){
                        float x = i + a - p;
                        float y = j + b - p;
                        float z = k + c - p;
                        float r = pow(x/float(r31), 2) + pow(y/float(r32), 2) + pow(z/float(r33), 2);
                        if (r <= 1.0){
                            if (etch) space[i+p][j+p][k+p] = 0;
                            if (grow) space[i+p][j+p][k+p] = 2;                            
                        }                  
                    }
                }
            }
        
        }        
    }
    statusText->append("Finished!");
    saveButton->setEnabled(true);
    fileField->blockSignals(false);
    checkFile();
    if (displayBox->isChecked()){
        
        drawPoints(p);
    }
}

void InputGen::saveClicked(){
    
    FILE * outFile;
    int num;
    
    statusText->append("Saving...");
    num = 0;
    outFile = fopen(fileField->text().toStdString().data(), "w");
    
    fputs("*** COMMENT LINE ***\n", outFile);
    for (int i = 0; i < space.size(); i++){
        for (int j = 0; j < space[i].size(); j++){
            for (int k = 0; k < space[i][j].size(); k++){
                if (space[i][j][k] > 0){
                    num = num + 1;
                }
            }
        }
    }
    fprintf(outFile,
            "    %d = NAT \n", num);
    fputs("  1.000000  0.000000  0.000000 = A_1 vector\n  0.000000  1.000000  0.000000 = A_2 vector\n", outFile);
    fputs("  1.000000  1.000000  1.000000 = lattice spacings (d_x,d_y,d_z)/d\n", outFile);
    fprintf(outFile,
            " %2.3f %2.3f %2.3f = X0(1-3) = location in lattice of center\n",
            -space.size()/2.0,
            -space[0].size()/2.0,
            -space[0][0].size()/2.0);
    fputs("     JA  IX  IY  IZ ICOMP(x,y,z)\n", outFile);
    num = 0;
    for (int i = 0; i < space.size(); i++){
        for (int j = 0; j < space[i].size(); j++){
            for (int k = 0; k < space[i][j].size(); k++){
                if (space[i][j][k] > 0){
                    num = num + 1;
                    fprintf(outFile,
                            "%7d %4d %4d %4d %2d %2d %2d\n",
                            num,
                            i+1,
                            j+1,
                            k+1,
                            1,
                            1,
                            1);
                }
            }
        }
    }
    cout << num;
    fclose(outFile);
    statusText->append("Saved!");
}

void InputGen::exitClicked(){
    
    exit(0);
}

void InputGen::drawPoints(int p){
    float color1[3] = {1.0f, 0.0f, 0.0f};
    float color2[3] = {0.0f, 0.0f, 1.0f};
    
    inputView->setVisible(true);

    for (int i = -p; i < p; i++){
        for (int j = -p; j < p; j++){
            for (int k = -p; k < p; k++){
                float coord[3] = {i/40.0, j/40.0, k/40.0};
                switch (space[i+p][j+p][k+p]){
                    case 1:
                        inputView->addSphere(coord, color1);
                        break;
                    case 2:
                        inputView->addSphere(coord, color2);
                        break;
                    default:
                        break;
                }
            }            
        }
    }
    inputView->updateGL();
    statusText->append("Done drawing!");
}


