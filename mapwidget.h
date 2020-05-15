#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QPoint>
#include <QString>
#include <QUrl>

#include <set>
#include <unordered_set>
#include <vector>

#include "mapeditorwidget.h"

extern int editorCmd;

class MapWidget: public QWidget
{
    Q_OBJECT
public:
    std::set<int> robotPos,rackPos,portPos,obsPos;
    std::unordered_set<int> occupiedPos;
    int scale;
    int width,height;
    MapWidget(QWidget* parent,
              int width = 20, int height = 20, int scale = 30);

    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

public slots:
    void onFileOpen();
    void onFileSave();

private:
    bool isFileOpend = false;
    QUrl mapFileUrl;

    int drag_start_block = -1, drag_end_block = -1;
    bool isDragValid = true;

    QPoint getCenterOfBlock(unsigned int block_id) const;
    QPoint getTopLeftOfBlock(unsigned int block_id) const;
    QPoint get2dBlock(unsigned int block_id) const;
    int get1dBlock(int x, int y) const;
    int getBlockOfPoint(QPoint& point)const ;
    void renderBlock(QPainter* painter, QBrush* brush, unsigned int block_id) const;

    QBrush obs{Qt::black};
    QBrush robot{Qt::red};
    QBrush rack{Qt::green};
    QBrush port{Qt::yellow};
};

#endif // MAPWIDGET_H
