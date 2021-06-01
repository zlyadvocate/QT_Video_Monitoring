#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QLabel>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>


class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
       explicit TreeWidget(QWidget *parent = nullptr);



protected:

    //鼠标压下事件,启动拖拽事件
    void mousePressEvent(QMouseEvent* event) override;

    //drag操作在窗体移动事件
    void dragMoveEvent(QDragMoveEvent* event) override;

    //drag操作进入窗体
    void dragEnterEvent(QDragEnterEvent* event) override;

    //drop释放事件
    void dropEvent(QDropEvent* event) override;
private:
    void GetRtspAddr(QString NVRID, QString IPCIP, QString &IPCRtspAddrMain, QString &IPCRtspAddrSub);
    QString GetNVRID(QString NVRIP);
    //拖放显示控件
    QLabel* label;

    //选中拖动节点
    QTreeWidgetItem* _selectItem;

};

#endif // TREEWIDGET_H
