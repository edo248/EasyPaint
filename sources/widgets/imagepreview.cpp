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

#include <QPainter>
#include <QPaintEvent>
#include <QImage>

#include "imagepreview.h"

ImagePreview::ImagePreview(QImage *image, QWidget *parent) :
    QWidget(parent), mFirstPoint(0, 0), mTopLeft(0, 0), mTopLeftCurrent(mTopLeft)
{
    mImage = image;
    mMoving = false;

    setCursor(Qt::OpenHandCursor);
}

void ImagePreview::paintEvent(QPaintEvent *event)
{
    if(mImage)
    {
        QPainter *painter = new QPainter(this);

        painter->drawImage(event->rect(), *mImage, QRect(mTopLeftCurrent, event->rect().size()));

        painter->end();
    }
}

void ImagePreview::mousePressEvent(QMouseEvent *event)
{
    mMoving = true;
    setCursor(Qt::ClosedHandCursor);

    mFirstPoint = event->pos();
}

void ImagePreview::mouseMoveEvent(QMouseEvent *event)
{
    if(!mMoving)
        return;

    mTopLeftCurrent = mTopLeft + mFirstPoint - event->pos();
    limitsImageBounds(mTopLeftCurrent);

    update();
}

void ImagePreview::mouseReleaseEvent(QMouseEvent *event)
{
    mMoving = false;
    setCursor(Qt::OpenHandCursor);

    mTopLeft += mFirstPoint - event->pos();
    limitsImageBounds(mTopLeft);
    mTopLeftCurrent = mTopLeft;
}

void ImagePreview::limitsImageBounds(QPoint &topLeft)
{
    if(topLeft.x() < 0)
        topLeft.setX(0);

    if(topLeft.y() < 0)
        mTopLeftCurrent.setY(0);

    if(topLeft.x() + rect().size().width() >= mImage->width())
        topLeft.setX(mImage->width() - rect().size().width());

    if(topLeft.y() + rect().size().height() >= mImage->height())
        topLeft.setY(mImage->height() - rect().size().height());
}
