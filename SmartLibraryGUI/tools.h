#ifndef TOOLS_H
#define TOOLS_H
#include <QPixmap>
#include <QImageReader>
#include <QPainter>
#include <QPainterPath>
namespace TOOLS
{
    QPixmap loadImage(const QString &path, const QSize &size);
    QPixmap getRoundPic(const QPixmap srcPixMap, const QSize & size, int radius);
    QPixmap getBlurredPic(const QImage &image,int radius, bool alphaOnly);
}


#endif // TOOLS_H
