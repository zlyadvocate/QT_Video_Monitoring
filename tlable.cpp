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


TLable::TLable(QWidget *parent):
QLabel(parent)
{
    setAcceptDrops(true);
    m_mpvwidget=new MpvWidget(this);
//    setCentralWidget(this);
    m_mpvwidget->m_id=winId();
    //zly
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(sizePolicy);
       //zly
        this->setAttribute(Qt::WA_Mapped);
    qDebug()<<"TLable::constructor";

//    qDebug()<<"class name is:"<<metaObject()->className()<<"objectName"<<objectName()<<text();


}

bool TLable::start(const QUrl &url)
{
    qDebug()<<"TLable::start";
    qDebug()<<"class name is:"<<metaObject()->className()<<"objectName"<<objectName()<<text();

    m_mpvwidget->m_id=winId();
    qDebug()<<"m_mpvwidget::mid"<< m_mpvwidget->m_id;
    return m_mpvwidget->start(url);


}

void TLable::audiomute()
{
    qDebug()<<text()<<objectName();
    if(m_mpvwidget)
        m_mpvwidget->audiomute();

}

void TLable::audioon()
{
   qDebug()<<"TLable::audioon()";

   qDebug()<<text()<<objectName();
    if(m_mpvwidget)
    m_mpvwidget->audioon();
}

void TLable::audiotoggle()
{
    if(m_mpvwidget)
    {

        if(audio_mute==false)
        {
            audio_mute=true;
            qDebug()<<"audio_mute=true";
            m_mpvwidget->audiomute();
        }else
        {
            audio_mute=false;
            qDebug()<<"audio_mute=false";
            m_mpvwidget->audioon();
        }
    }
}

void TLable::screenshot()
{
    if(m_mpvwidget)
        m_mpvwidget->screenshot();
}

void TLable::enablelog()
{
    if(m_mpvwidget)
        m_mpvwidget->enablelog();
}

void TLable::showEvent(QShowEvent *e)
{
    setAttribute(Qt::WA_Mapped);
    QWidget::showEvent(e);
}

void TLable::resizeEvent(QResizeEvent *event)
{
    qDebug()<<" TLable::resizeEvent";

}

void TLable::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText())
    {
        QString mydata=event->mimeData()->text();
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
