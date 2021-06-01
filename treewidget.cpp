#include "treewidget.h"
#include <QMimeData>
#include <QDataStream>
#include <QDrag>
#include <QDebug>
#include <QSqlQuery>

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
        qDebug()<<_selectItem->text(0)<<"\n";

        QString m_select=_selectItem->parent()->text(0);

        QString NVRIP = m_select.split("[")[1].split("]")[0];
        QString NVRID = GetNVRID(NVRIP);
        qDebug()<<"NVRIP main"<<NVRIP<<"NVRID is:"<<NVRID<<"\n";
        QString temp=_selectItem->text(0);
        QString IPCIP = temp.split("[")[1].split("]")[0];
        qDebug()<<"temp1 main"<<temp<<"IPCIP is:"<<IPCIP<<"\n";
        //根据NVR编号和IP地址查询出该摄像机的子码流
        QString rtspAddr;
        QString IPCRtspAddrMain;
        QString IPCRtspAddrSub;
        GetRtspAddr(NVRID, IPCIP, IPCRtspAddrMain, IPCRtspAddrSub);
        qDebug()<<"rtsp main"<<IPCRtspAddrMain<<"sub is:"<<IPCRtspAddrSub<<"\n";

        //设置自定义数据
        QMimeData* mimeData = new QMimeData;
        //        mimeData->setData("text", dataItem);
        mimeData->setText( IPCRtspAddrMain);

        //拖动设置
        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);


        Qt::DropAction dropAction = drag->exec(Qt::MoveAction|Qt::CopyAction,Qt::CopyAction);

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

void TreeWidget::GetRtspAddr(QString NVRID, QString IPCIP, QString &IPCRtspAddrMain, QString &IPCRtspAddrSub)
{
    QSqlQuery query;
    QString sql = "select [IPCRtspAddrMain],[IPCRtspAddrSub] from [IPCInfo] where [IPCUse]='启用'";
    sql += " and [NVRID]='" + NVRID + "'";
    query.exec(sql);
    while(query.next()) {
        //取出子码流地址,看是否IP相同
        QString rtspAddr = query.value(0).toString();
        QStringList temp = rtspAddr.split("/");
        QString ip = temp[2].split(":")[0];
        if (ip == IPCIP) {
            IPCRtspAddrMain = query.value(0).toString();
            IPCRtspAddrSub = query.value(1).toString();
            break;
        }
    }
}

QString TreeWidget::GetNVRID(QString NVRIP)
{
    QSqlQuery query;
    QString sql = "select [NVRID] from [NVRInfo]";
    sql += " where [NVRIP]='" + NVRIP + "'";
    query.exec(sql);
    query.next();
    return query.value(0).toString();
}
