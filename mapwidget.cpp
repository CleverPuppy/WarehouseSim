#include "mapwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <iostream>
#include <algorithm>
#include <vector>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <fstream>

MapWidget::MapWidget(QWidget* parent,
          int width, int height, int scale)
    :width(width),height(height),scale(scale),QWidget(parent)
{

    setFixedSize(width * scale + 10,height * scale + 10);
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

//     render grid line
    auto bg = QBrush(QColor(46,13,68));
    painter.setBackground(bg);

    QPen grid;
    grid.setColor(Qt::lightGray);
    grid.setWidth(1);
    painter.setPen(grid);
    for(size_t i=0; i <= width + 1; ++i)
    {
        QPoint top(i,0),bottom(i,int(height));

        painter.drawLine(top * scale, bottom * scale);
    }
    for(size_t i=0; i<=height + 1;++i)
    {
        QPoint left(0,i),right(int(width),i);
        painter.drawLine(left * scale, right*scale);

    }

    QBrush b_robot(Qt::red),b_rack(Qt::blue),b_obs(Qt::black), b_port(Qt::green);
    for(auto id: robotPos)
    {
        renderBlock(&painter,&b_robot,id);
    }
    for(auto id: rackPos)
    {
        renderBlock(&painter,&b_rack,id);
    }
    for(auto id: obsPos)
    {
        renderBlock(&painter,&b_obs,id);
    }
    for(auto id: portPos)
    {
        renderBlock(&painter,&b_port,id);
    }

    //paint drag
    if(drag_start_block != -1 && drag_end_block != -1)
    {
        QBrush dragBrush = QBrush(Qt::lightGray);
        QPoint _start,_end;
        painter.setBrush(dragBrush);
        painter.setOpacity(0.4f);
        _start = get2dBlock(drag_start_block);
        _end = get2dBlock(drag_end_block);
        auto x_pair = std::minmax(_start.x(), _end.x());
        auto y_pair = std::minmax(_start.y(), _end.y());

        painter.drawRect(x_pair.first * scale,
                         y_pair.first * scale,
                         (x_pair.second - x_pair.first + 1)*scale,
                         (y_pair.second - y_pair.first + 1)*scale);
        painter.setOpacity(1.0);
    }
    painter.end();
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    //render a rectangle
    auto pos = event->pos();
    int block_id = getBlockOfPoint(pos);
    if(block_id != -1 || block_id == drag_end_block)
    {

        drag_end_block = block_id;
        this->update();
    }
    else{

    }
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        auto Pos = event->pos();
        int block_id = getBlockOfPoint(Pos);
        if(block_id != -1 )
        {
            std::cout<<"press block " <<block_id<<std::endl;
            drag_start_block = block_id;
        }
        else{
            std::cout <<" point not in a block : (" << Pos.x() << ","
                     << Pos.y() <<")"<<std::endl;
        }
    }

    if(event -> button() == Qt::MouseButton::RightButton)
    {
        //
        drag_start_block = drag_end_block = -1;

    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    std::vector<int> allBlockId;
    //click mode
    auto pos = event->pos();
    unsigned int block_id = getBlockOfPoint(pos);
    if(block_id != -1 && block_id == drag_start_block)
    {
        allBlockId.emplace_back(block_id);
    }
    //drag mode
    if(isDragValid && drag_start_block != -1 && drag_end_block != -1)
    {
        QPoint _s,_end;
        _s = get2dBlock(drag_start_block);
        _end = get2dBlock(drag_end_block);
        auto x_pair = std::minmax(_s.x(),_end.x());
        auto y_pair = std::minmax(_s.y(), _end.y());

        for(int min_x = x_pair.first,max_x = x_pair.second; min_x <= max_x; ++ min_x )
        {
            for(int min_y = y_pair.first, max_y = y_pair.second; min_y <= max_y; ++ min_y)
            {
                allBlockId.emplace_back(get1dBlock(min_x,min_y));
            }
        }
    }

    //insert  or remove
        if(editorCmd == (int)EditorCmd::ADD_ROBOT)
        {
            for(auto id: allBlockId)
            {
//                    robotPos.erase(id);
                rackPos.erase(id);
                obsPos.erase(id);
                portPos.erase(id);
            }
            robotPos.insert(allBlockId.begin(),allBlockId.end());
        }
        if(editorCmd == (int)EditorCmd::ADD_RACK)
        {
            for(auto id: allBlockId)
            {
                robotPos.erase(id);
//                    rackPos.erase(id);
                obsPos.erase(id);
                portPos.erase(id);
            }
            rackPos.insert(allBlockId.begin(),allBlockId.end());
        }
        if(editorCmd == (int)EditorCmd::ADD_OBSTACLE)
        {
            for(auto id: allBlockId)
            {
                robotPos.erase(id);
                rackPos.erase(id);
//                    obsPos.erase(id);
                portPos.erase(id);
            }
            obsPos.insert(allBlockId.begin(),allBlockId.end());
        }
        if(editorCmd == (int)EditorCmd::ADD_PORT)
        {
            for(auto id: allBlockId)
            {
                robotPos.erase(id);
                rackPos.erase(id);
                obsPos.erase(id);
//                    portPos.erase(id);
            }
            portPos.insert(allBlockId.begin(),allBlockId.end());
        }
        if(editorCmd == (int)EditorCmd::REMOVE)
        {
            for(auto id: allBlockId)
            {
                robotPos.erase(id);
                rackPos.erase(id);
                obsPos.erase(id);
                portPos.erase(id);
            }
        }

        // clear states and repaint
        drag_start_block = drag_end_block = -1;
        this->update();
}
void MapWidget::dragMoveEvent(QDragMoveEvent *event)
{
    std::cout<<"DragMoveEvent emit"<<std::endl;
}

QPoint MapWidget::getCenterOfBlock(unsigned int block_id) const
{
    unsigned int max = width * height;
    unsigned int x=0,y=0;
    if(block_id <= max)
    {
        y = block_id / width;
        x = block_id % width;
    }
    else{
        std::cout << "ERROR::MAPWIDGET::getCenterOfBlock::Out of block_id"<<std::endl;
    }

    //transform to scaled center
    return QPoint(x + scale / 2, y+ scale/ 2);
}

QPoint MapWidget::getTopLeftOfBlock(unsigned int block_id) const
{
    unsigned int max = width * height;
    unsigned int x=0,y=0;
    if(block_id <= max)
    {
        y = block_id / width;
        x = block_id % width;
    }
    else{
        std::cout << "ERROR::MAPWIDGET::getTopLeftOfBlock::Out of block_id"<<std::endl;
    }
    return QPoint(x * scale ,y * scale);
}

QPoint MapWidget::get2dBlock(unsigned int block_id) const
{
    unsigned int max = width * height;
    unsigned int x=0,y=0;
    if(block_id <= max)
    {
        y = block_id / width;
        x = block_id % width;
    }
    else{
        std::cout << "ERROR::MAPWIDGET::getTopLeftOfBlock::Out of block_id"<<std::endl;
    }
    return QPoint(x,y);
}

int MapWidget::get1dBlock(int x, int y) const
{
    return y * width + x;
}

int MapWidget::getBlockOfPoint(QPoint &point) const
{
    int x = point.x();
    int y = point.y();
    int max_x = width * scale;
    int max_y = height * scale;
    if(x <= max_x && x >=0 && y <= max_y && y >=0)
    {
        int block_id = (y / scale) * width + (x / scale);
        return block_id;
    }else{
        return -1;
    }
}

void MapWidget::renderBlock(QPainter *painter, QBrush *brush, unsigned int block_id) const
{
    painter ->setBrush(*brush);

    unsigned int x = (block_id % width ) * scale;
    unsigned int y = (block_id / width) * scale;
    unsigned int w,h;
    w = h = scale;
    painter ->drawRect(x,y,w,h);
}

void MapWidget::onFileOpen()
{
   auto _map = QFileDialog::getOpenFileUrl(this,
                                                "选择地图文件",
                                                QUrl("./"),
                                                "Files (* *.txt)");
   if(_map.isEmpty())
   {
       // unopend

   }else{
       isFileOpend = true;
       QFile file(_map.fileName());
       file.open(QIODevice::ReadOnly);
       QTextStream ifs(&file);
       int robotSize = -1,rackSize = -1 ,obsSize = -1,portSize = -1;

           ifs >> width >> height >> robotSize >> rackSize >> obsSize >> portSize;
           for(int i=0 ; i< robotSize; ++i)
           {
                int v;
                ifs >> v;
                robotPos.insert(v);
           }
           for(int i=0 ; i< rackSize; ++i)
           {
                int v;
                ifs >> v;
                rackPos.insert(v);
           }
           for(int i=0 ; i< obsSize; ++i)
           {
                int v;
                ifs >> v;
                obsPos.insert(v);
           }
           for(int i=0 ; i< portSize; ++i)
           {
                int v;
                ifs >> v;
                portPos.insert(v);
           }
           this->update();

   }


}

void MapWidget::onFileSave()
{

    auto _map = QFileDialog::getSaveFileUrl(this,
                                             "保存",
                                             isFileOpend?QUrl(mapFileUrl):QUrl("./"),
                                             "*.txt");

    if(_map.isEmpty())
    {
        // unsaved
    }
    else{
           // saving
            QFile file(_map.fileName());
            file.open(QIODevice::WriteOnly);
            QTextStream ofs(&file);
            std::cout << "saving to " << _map.toString().toStdString() << std::endl;

                ofs << width << " "
                    <<height <<  " "
                    << robotPos.size() << " "
                    << rackPos.size() <<" "
                    << obsPos.size() <<" "
                    << portPos.size() <<" "
                    <<"\n";
                for(auto id : robotPos)
                    ofs << id << " ";
                ofs << "\n";
                for(auto id : rackPos)
                    ofs << id << " ";
                ofs << "\n";
                for(auto id : obsPos)
                    ofs << id << " ";
                ofs << "\n";
                for(auto id : portPos)
                    ofs << id << " ";
                ofs << "\n";
                mapFileUrl = _map;
                isFileOpend = true;


    }

}
