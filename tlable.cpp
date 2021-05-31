#include "tlable.h"
#include <QDebug>
#include <QByteArray>
#include <QMimeData>
#include <QDataStream>
#include <QTextCodec>

QString GetCorrectUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode(ba.constData(), ba.size(), &state);
    if (state.invalidChars > 0)
    {
        text = QTextCodec::codecForName("GBK")->toUnicode(ba);
    }
    else
    {
        text = ba;
    }
    return text;
}
//————————————————
//版权声明：本文为CSDN博主「阳光柠檬_」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/liukang325/article/details/80986472

TLable::TLable(QWidget *parent):
QLabel(parent)
{
    setAcceptDrops(true);
}

void TLable::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText())
    {
        QString mydata=event->mimeData()->text();
//        qDebug()<<"TLable::dragEnterEvent text is:"<<mydata;
         event->acceptProposedAction();

    }

}

void TLable::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasText())
    {
        QString mydata=event->mimeData()->text();
//        qDebug()<<"TLable::dropEvent text is:"<<mydata;
        //update MPV url
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();

}
