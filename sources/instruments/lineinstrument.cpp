/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "lineinstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"

#include <QPen>
#include <QPainter>
#include <QImage>

LineInstrument::LineInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
    mIsSecondaryColor = false;
}

void LineInstrument::mousePressEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        setStartPoint(event->pos());
        setEndPoint(event->pos());

        if (!imageArea.isPaint())
        {
            if(event->button() == Qt::LeftButton)
            {
                setIsSecondaryColor(false);
            }
            else if(event->button() == Qt::RightButton){
                setIsSecondaryColor(true);
            }
        }

        imageArea.setIsPaint(true); //TODO: move to init
        initInstrumentAction(imageArea);
    }
}

void LineInstrument::mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(imageArea.isPaint())
    {
        setEndPoint(event->pos());
        imageArea.setImage(mImageCopy);
        paint(imageArea, mIsSecondaryColor);
    }
}

void LineInstrument::mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(imageArea.isPaint())
    {        
        completeInstrumentAction(imageArea);
        imageArea.setIsPaint(false); //TODO: move to complete
    }
}

void LineInstrument::paint(ImageArea &imageArea, bool isSecondaryColor, bool)
{
    QPainter painter(imageArea.getImage());
    if(isSecondaryColor)
    {
        painter.setPen(QPen(DataSingleton::Instance()->getSecondaryColor(),
                            DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
                            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    else
    {
        painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor(),
                            DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
                            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    if(mStartPoint != mEndPoint)
    {
        painter.drawLine(mStartPoint, mEndPoint);
    }

    if(mStartPoint == mEndPoint)
    {
        painter.drawPoint(mStartPoint);
    }
    imageArea.setEdited(true);
    //    int rad(DataSingleton::Instance()->getPenSize() + round(sqrt((mStartPoint.x() - mEndPoint.x()) *
    //                                                                 (mStartPoint.x() - mEndPoint.x()) +
    //                                                                 (mStartPoint.y() - mEndPoint.y()) *
    //                                                                 (mStartPoint.y() - mEndPoint.y()))));
    //    mPImageArea->update(QRect(mStartPoint, mEndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    painter.end();
    imageArea.update();
}

void LineInstrument::setStartPoint(QPoint inPoint)
{
    mStartPoint = inPoint;
}

void LineInstrument::setEndPoint(QPoint inPoint)
{
    mEndPoint = inPoint;
}

void LineInstrument::completeInstrumentAction(ImageArea &imageArea)
{
    imageArea.setImage(mImageCopy);
    paint(imageArea, mIsSecondaryColor);
}

void LineInstrument::initInstrumentAction(ImageArea &imageArea)
{

    mImageCopy = *imageArea.getImage();
    makeUndoCommand(imageArea);
}


void LineInstrument::setIsSecondaryColor(bool secondary)
{
    mIsSecondaryColor = secondary;
}
