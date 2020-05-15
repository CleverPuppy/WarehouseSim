#include "mapeditorwidget.h"
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <iostream>
#include "mapwidget.h"
int editorCmd;
MapEditorWidget::MapEditorWidget(QWidget* parent):
    QWidget(parent)
{
//    setMinimumWidth(200);
//    setMinimumHeight(600);

    // 绘制edit部分
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* fileHint = new QLabel("File",this);
    QPushButton* openFile = new QPushButton("Open",this);
    QPushButton* saveFile = new QPushButton("Save",this);

    connect(openFile,&QPushButton::clicked,this,[=](){
        emit onOpenFile();
    });
    connect(saveFile,&QPushButton::clicked,this,[=](){
        emit onSaveFile();
    });

    layout->addWidget(fileHint);
    layout->addWidget(openFile);
    layout->addWidget(saveFile);


    QButtonGroup* btnGroup = new QButtonGroup(this);
    QPushButton* addRobot = new QPushButton("Robot",this);
    QPushButton* addRack = new QPushButton("Rack",this);
    QPushButton* addObstacle = new QPushButton("Obstacle",this);
    QPushButton* addPort = new QPushButton("Port",this);
    QPushButton* remove = new QPushButton("Remove",this);

    QLabel* hint = new QLabel(tr("当前操作:"),this);
    QLabel* currentCmd = new QLabel(this);

    btnGroup->addButton(addRobot,1);
    btnGroup->addButton(addRack,2);
    btnGroup->addButton(addObstacle,3);
    btnGroup->addButton(addPort,4);
    btnGroup->addButton(remove,5);
    connect(btnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            [=](int id){
        std::cout <<"clicked id: " <<id <<std::endl;
        switch(id)
        {
        case int(EditorCmd::ADD_ROBOT):
            currentCmd->setText(tr("ADD ROBOT"));
            editorCmd = (int)EditorCmd::ADD_ROBOT;
            break;
        case int(EditorCmd::ADD_RACK):
            currentCmd->setText(tr("ADD RACk"));
            editorCmd =(int) EditorCmd::ADD_RACK;
            break;
        case int(EditorCmd::ADD_OBSTACLE):
            currentCmd->setText(tr("ADD OBSTACLE"));
            editorCmd =(int) EditorCmd::ADD_OBSTACLE;
            break;
        case int(EditorCmd::ADD_PORT):
            currentCmd->setText(tr("ADD PORT"));
            editorCmd =(int) EditorCmd::ADD_PORT;
            break;
        case int(EditorCmd::REMOVE):
            currentCmd->setText(tr("REMOVE"));
            editorCmd =(int) EditorCmd::REMOVE;
            break;
        }
    });

    layout -> addWidget(hint);
    layout -> addWidget((currentCmd));
    layout -> addWidget(addRobot);
    layout -> addWidget(addRack);
    layout -> addWidget(addObstacle);
    layout -> addWidget(addPort);

    layout -> addWidget(remove);
    setLayout(layout);
}
