#ifndef TLABLE_H
#define TLABLE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "mpvwidget.h"


class TLable : public QLabel
{
 Q_OBJECT
public:

          explicit TLable(QWidget *parent = nullptr);
      bool start(const QUrl &url);

protected:



    //drag操作进入窗体
    void dragEnterEvent(QDragEnterEvent* event) override;

    //drop释放事件
    void dropEvent(QDropEvent* event) override;
private:
    MpvWidget* m_mpvwidget;
    Q_DISABLE_COPY(TLable)

};

#endif // TLABLE_H
