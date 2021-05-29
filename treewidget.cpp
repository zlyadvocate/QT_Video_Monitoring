#include "treewidget.h"
#include <QMimeData>
#include <QDataStream>
#include <QDrag>
#include <QDebug>

#include <QByteArray>


TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{

}

void TreeWidget::mousePressEvent(QMouseEvent *event)
{
    _selectItem= static_cast<QTreeWidgetItem*>(this->itemAt(event->pos()));
    if (event->button()==Qt::LeftButton&&_selectItem){
    QByteArray dataItem;
        QDataStream dataStream(&dataItem, QIODevice::WriteOnly);
        dataStream << _selectItem->text(0);
//        label->setText(_selectItem->text(0));
        qDebug()<<_selectItem->text(0)<<"\n";

        //设置自定义数据
        QMimeData* mimeData = new QMimeData;
        mimeData->setData("text", dataItem);
//        qDebug() <<mimeData->data();

        //拖动设置
        QDrag* drag = new QDrag(this);
//        drag->setPixmap(label->grab());
        drag->setMimeData(mimeData);
//        drag->setHotSpot(QPoint(label->grab().width() / 2, label->grab().height() / 2));

        Qt::DropAction dropAction = drag->exec(Qt::MoveAction|Qt::CopyAction,Qt::CopyAction);
        if(dropAction == Qt::MoveAction){
            //当成功拖动时清除原节点
//            delete _selectItem;
//            _selectItem = NULL;
        }
    }
    QTreeWidget::mousePressEvent(event);
}


void TreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
  //判断自定义类型
   if (event->mimeData()->hasFormat("Data/name")){
        //children()返回所有子对象,ev->sourve返回当前拖拽的对象
        if(children().contains(event->source())){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
   }
}

void TreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget *source =  qobject_cast<TreeWidget *>(event->source());
       if (source /*&& source != this*/)
       {
//           if (m_ctrlPressed)
//           {
//               event->setDropAction(Qt::CopyAction);
//           }
//           else
           {
               event->setDropAction(Qt::MoveAction);
           }
           /*event->setDropAction(Qt::MoveAction);  */
           event->accept();
       }


}

void TreeWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("Data/name")) {
        //获取拖拽的数据
        QByteArray itemData = event->mimeData()->data("Data/name");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QString text;
        dataStream >>text;

        //获取放置的父节点
        QTreeWidgetItem *item = itemAt(event->pos());
        if (item == nullptr) {
            return;
        }else if(item == _selectItem){
             return;
        }
        //创建新节点
//        QTreeWidgetItem* newItem = new QTreeWidgetItem(item);
//        newItem->setText(0, text);
//        item->setExpanded(true);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}
