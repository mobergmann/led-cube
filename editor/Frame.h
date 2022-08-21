//
// Created by mobergmann on 21.08.22.
//

#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include <QPainter>

#include "Cube.h"


class Frame: public QWidget
{
private:
    int time;
    Cube cube;

protected:
    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 12, Qt::DashDotLine, Qt::RoundCap));
        painter.drawLine(0, 0, 200, 200);

        for (const auto &a1: cube.points)
        {
            for (const auto &a2: a1)
            {
                for (const auto &point: a2)
                {
                    render(point);
                }
            }
        }
    }
};


#endif //FRAME_H
