#ifndef MAPEDITORWIDGET_H
#define MAPEDITORWIDGET_H

#include <QPainter>
#include <QWidget>

enum class EditorCmd
{
    ADD_ROBOT =1,
    ADD_RACK = 2,
    ADD_OBSTACLE = 3,
    ADD_PORT = 4,
    REMOVE = 5
};

extern int editorCmd;

class MapEditorWidget : public QWidget
{
    Q_OBJECT
public:
    MapEditorWidget(QWidget* parent);

signals:
    void onOpenFile();
    void onSaveFile();

};

#endif // MAPEDITORWIDGET_H
