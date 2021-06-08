#include "mpvwidget.h"
#include <QDebug>
#include <QUrl>

MpvWidget::MpvWidget(QWidget *parent) : QWidget(parent)
{

}

MpvWidget::~MpvWidget()
{
    if (m_mpv)
        mpv_terminate_destroy(m_mpv);

}

bool MpvWidget::start(const QUrl &url)
{


    qDebug() << "MpvVideoPlayerBackend::start url =" << url << "\n";
    //        if (state() == PermanentError)
    //            return false;

    if (!m_id)
    {
        //            setError(false, tr("Window id is not set"));
        qDebug() << "Window id is not set"  << "\n";
        return false;
    }

    //            if (m_mpv)
    //            {
    //                int pause = 1;
    //                mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &pause);
    //                mpv_terminate_destroy(m_mpv);
    //                m_mpv = NULL;
    //            }
    qDebug() << "MpvWidget: start create MPV \n";

    if (!createMpvProcess())
    {
        qDebug() << "MpvVideoPlayerBackend: start - failed to create MPV \n";
        return false;
    }


    //    m_playbackSpeed = 1.0;
    //  qDebug() << "MpvWidget:: MPV createMpvProcess sucess\n";



    QString n=url.toString();
    QByteArray bate = n.toLocal8Bit();
    const char *c_str24=bate.data();

    qDebug()<<"url c_str24"<<c_str24;
    const char *args[] = { "loadfile", c_str24, NULL };
    mpv_command(m_mpv, args);//ok

    return true;
}

void MpvWidget::receiveMpvEvents()
{
    // This slot is invoked by wakeup() (through the mpv_events signal).
    while (m_mpv)
    {
        mpv_event *event = mpv_wait_event(m_mpv, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;
        handleMpvEvent(event);
    }
}

bool MpvWidget::createMpvProcess()
{
    if (!m_mpv)
    {
        qDebug() << "MpvVideoPlayerBackend: Can't create mpv instance!\n";
        return false;
    }

    m_mpv = mpv_create();
    qDebug() << "MpvWidget::createMpvProcess() ______start!\n";



    mpv_set_option(m_mpv, "wid", MPV_FORMAT_INT64, &m_id);

    {
        //        QSettings settings;
        //                QString vo =  QLatin1String("opengl,direct3d,sdl");
        QString vo =  QLatin1String("opengl,direct3d,sdl");


        //        if (vo == QLatin1String("default"))
#ifdef Q_OS_WIN
        vo = QLatin1String("direct3d");
#else
        //            vo = QLatin1String("");
#endif
        mpv_set_option_string(m_mpv, "vo", "direct3d");

    }
    //请求级别日志消息
      mpv_request_log_messages(m_mpv, "info");

    mpv_set_option_string(m_mpv, "mute", "no");
    mpv_set_option_string(m_mpv, "hwdec", "auto");

    mpv_set_option_string(m_mpv, "input-default-bindings", "yes");
    mpv_set_option_string(m_mpv, "input-vo-keyboard", "yes");
    mpv_set_option_string(m_mpv, "input-cursor", "no");
    mpv_set_option_string(m_mpv, "cursor-autohide", "no");

    mpv_observe_property(m_mpv, 0, "percent-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "pause", MPV_FORMAT_FLAG);

//    mpv_request_log_messages(m_mpv, "error");  // no fatal error warn info v debug trace
     mpv_request_log_messages(m_mpv, "info");  // no fatal error warn info v debug trace

    connect(this, SIGNAL(mpvEvents()), this, SLOT(receiveMpvEvents()), Qt::QueuedConnection);
    //    mpv_set_wakeup_callback(m_mpv, wakeup, this);
    qDebug() << "MpvWidget::mpv_initialize start!\n";
    if (mpv_initialize(m_mpv) < 0)
    {
        qDebug() << "MpvVideoPlayerBackend: mpv failed to initialize!\n";
        return false;
    }
    audiomute();//

    return true;
}


void MpvWidget::handleMpvEvent(mpv_event *event)
{
    if (event->event_id == MPV_EVENT_NONE)
        return;

    switch (event->event_id)
    {
    case MPV_EVENT_FILE_LOADED:
        m_state = VIDEO_PLAYING;
        showText(QByteArrayLiteral(" m_state = VIDEO_PLAYING..."));
        break;

    case MPV_EVENT_PROPERTY_CHANGE:
    {
        mpv_event_property *prop = (mpv_event_property *)event->data;
        if (strcmp(prop->name, "percent-pos") == 0)
        {
            if (prop->format == MPV_FORMAT_DOUBLE)
            {

            }
        }
        else if (strcmp(prop->name, "time-pos") == 0)
        {
            if (prop->format == MPV_FORMAT_DOUBLE)
            {
                //                       m_timepos = *(double *)prop->data;
            }
        }
        else if (strcmp(prop->name, "duration") == 0)
        {
            if (prop->format == MPV_FORMAT_DOUBLE)
            {
                double duration = *(double *)prop->data;

            }
        }
        else if (strcmp(prop->name, "pause") == 0)
        {
            if (m_state == VIDEO_PLAYING)
            {
                m_state = VIDEO_PAUSED;
                showText(QByteArrayLiteral(" m_state = VIDEO_PAUSED..."));
            }
            else if (m_state == VIDEO_PAUSED)
            {
                m_state = VIDEO_PLAYING;
                showText(QByteArrayLiteral(" m_state = VIDEO_PLAYING..."));
            }
            //            emit stateChanged();
        }

        else if (strcmp(prop->name,"paused-for-cache") == 0)
        {
            if ( m_state != STOPPED)
            {
                showText(QByteArrayLiteral("Network is slow..."));
            }
            else
            {
                showText(QByteArrayLiteral(""));
            }
        }

        else if (strcmp(prop->name, "core-idle") == 0)
        {
            if ( m_state == VIDEO_PLAYING)
            {
                showText(QByteArrayLiteral("Buffering..."));
            }
            else
            {
                showText(QByteArrayLiteral(""));
            }
        }

        break;
    }
    case MPV_EVENT_SEEK:
    {
        break;
    }
    case MPV_EVENT_END_FILE:
    {
        mpv_event_end_file *eef = (mpv_event_end_file*) event->data;
          showText(QByteArrayLiteral("MPV_EVENT_END_FILE..."));
        switch (eef->reason)
        {
        case MPV_END_FILE_REASON_EOF:
        {
            //                       handleEof();
            break;
        }
        case MPV_END_FILE_REASON_STOP:
        case MPV_END_FILE_REASON_QUIT:
        case MPV_END_FILE_REASON_ERROR:
        case MPV_END_FILE_REASON_REDIRECT:
            break;
            //        default: m_position = -1;
        }

        break;
    }
    case MPV_EVENT_LOG_MESSAGE:
    {
        mpv_event_log_message *msg = (mpv_event_log_message*) event->data;

        qDebug() << "MpvVideoPlayerBackend: [" << msg->prefix << "] "
                 << msg->level << ": " << msg->text;
        break;
    }
    case MPV_EVENT_SHUTDOWN:
    {
        qDebug() << "MpvVideoPlayerBackend: MPV shutdown event";

        mpv_terminate_destroy(m_mpv);
        m_mpv = NULL;
        break;
    }
    default: ; // Ignore uninteresting or unknown events.
    }
}

///////////////////////////////////////////////////////////
///
///




///



void MpvWidget::showText(const QByteArray &text)
{
    const char *args[] = {"show-text", text.constData(), nullptr};
    uint64_t reply_userdata;
    mpv_command_async(m_mpv,reply_userdata,args);

    //    m_mpv.command_async(args);
}

void MpvWidget::handleMpvError(int code)
{
    if (code < 0)
    {
        qDebug() << "MPV Error: " << mpv_error_string(code);
    }

}

void MpvWidget::audiomute()
{
    showText(QByteArrayLiteral(" audiomute"));

    if (m_mpv)
    {
        qDebug() << "MpvWidget audiomute:";
//        mpv_set_option_string(m_mpv, "mute", "yes");
          setVolume(0);
    }

}

void MpvWidget::audioon()
{
    showText(QByteArrayLiteral("audioon"));
    if (m_mpv)
    {

//        mpv_set_option_string(m_mpv, "mute", "no");
        setVolume(80);
        qDebug() << "MpvWidget audio_____on: volumn 80";
    }
}

void MpvWidget::screenshot()
{
    showText(QByteArrayLiteral(" screenshot"));
    if (m_mpv)
    {
        qDebug() << "MpvWidget screenshot:";
        uint64_t reply_userdata;
        //         if (m_state == STOPPED)
        //             return;
        const char *args[] = {"osd-msg", "screenshot", nullptr};
        mpv_command_async(m_mpv,reply_userdata,args);

    }
}

void MpvWidget::setVolume(int volume)
{
    //    if (m_volume == volume)
    //        return;

    //    m_volume = volume;
    uint64_t reply_userdata;
    double mdara=static_cast<double>(volume);
    void *p=&mdara;

//    m_set_property_async(reply_userdata,"volume", MPV_FORMAT_DOUBLE,p);
    m_set_property("volume", MPV_FORMAT_DOUBLE,p);

    showText(QByteArrayLiteral("Volume: ") + QByteArray::number(volume));

}

int MpvWidget::m_command_async(uint64_t reply_userdata, const char **args)
{
   return  mpv_command_async(m_mpv,reply_userdata,args);
}

int MpvWidget::m_set_property(const char *name, mpv_format format, void *data)
{
    return mpv_set_property(m_mpv,name,format,data);
}

int MpvWidget::m_set_property_string(const char *name, const char *data)
{
    return mpv_set_property_string(m_mpv,name,data);
}

int MpvWidget::m_set_property_async(uint64_t reply_userdata, const char *name, mpv_format format, void *data)
{
   return  mpv_set_property_async(m_mpv,reply_userdata,name,format,data);
}

int MpvWidget::m_get_property(const char *name, mpv_format format, void *data)
{
    return mpv_get_property(m_mpv,name,format,data);
}

char *MpvWidget::m_get_property_string(const char *name)
{
    return mpv_get_property_string(m_mpv,name);
}

void MpvWidget::enablelog()
{
    mpv_set_option_string(m_mpv, "log-file", "g:\\mov\\mpvbin\\123.log");

}
