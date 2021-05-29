#ifndef TLABLE_H
#define TLABLE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>


class TLable : public QLabel
{
public:

          explicit TLable(QWidget *parent = nullptr);
protected:



    //drag操作进入窗体
    void dragEnterEvent(QDragEnterEvent* event) override;

    //drop释放事件
    void dropEvent(QDropEvent* event) override;
private:

};

#endif // TLABLE_H
