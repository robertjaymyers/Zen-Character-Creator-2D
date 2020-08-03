#pragma once

#include <QGraphicsPixmapItem>

class PixmapItemAnimatable : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

public:
	PixmapItemAnimatable(QGraphicsItem *parent = nullptr);
};