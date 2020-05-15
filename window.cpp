#include "window.h"
#include "mapwidget.h"
#include "mapeditorwidget.h"
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

Window::Window()
{

    setWindowTitle(tr("Warehouse Map Editor"));

//    QMenuBar* menuBar = new QMenuBar(this);
//    menuBar->setGeometry(0,0,width(),40);
//    QMenu* fileMenu = new QMenu(tr("File"),this);

//    QAction* fileOpenAction = new QAction("Open",this);
//    QAction* fileSaveAction = new QAction("Save",this);

//    fileMenu->addAction(fileOpenAction);
//    fileMenu->addAction(fileSaveAction);

//    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    mainLayout->addWidget(menuBar);

    QScrollArea* mapScroll = new QScrollArea(this);
    mapScroll->setMinimumWidth(800);
    mapScroll->setMinimumHeight(600);
    MapEditorWidget* mapEditor = new MapEditorWidget(this);
    MapWidget* mapWidget = new MapWidget(this);
    mapScroll->setWidget(mapWidget);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(mapScroll);
    layout->addWidget(mapEditor);

    connect(mapEditor,&MapEditorWidget::onOpenFile,mapWidget,&MapWidget::onFileOpen);
    connect(mapEditor,&MapEditorWidget::onSaveFile,mapWidget,&MapWidget::onFileSave);
    setLayout(layout);

}
