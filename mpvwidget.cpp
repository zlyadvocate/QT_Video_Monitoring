#include "mpvwidget.h"
#include <QDebug>

MpvWidget::MpvWidget(QWidget *parent) : QWidget(parent)
{

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

    //        if (m_mpv)
    //        {
    //            int pause = 1;
    //            mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &pause);
    //            mpv_terminate_destroy(m_mpv);
    //            m_mpv = NULL;
    //        }
    qDebug() << "MpvWidget: start create MPV \n";

    if (!createMpvProcess())
    {
        qDebug() << "MpvVideoPlayerBackend: start - failed to create MPV \n";
        return false;
    }



//    m_playbackSpeed = 1.0;
    qDebug() << "MpvWidget:: MPV createMpvProcess sucess\n";

    const char *args[] = { "loadfile", "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov", NULL };
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
    m_mpv = mpv_create();
    qDebug() << "qvideowidget::createMpvProcess() start!\n";

    if (!m_mpv)
    {
        qDebug() << "MpvVideoPlayerBackend: Can't create mpv instance!\n";
        return false;
    }

    mpv_set_option(m_mpv, "wid", MPV_FORMAT_INT64, &m_id);

    {
        //        QSettings settings;
                QString vo =  QLatin1String("opengl,direct3d,sdl");

        //        if (vo == QLatin1String("default"))
        //    #ifdef Q_OS_WIN
//                    vo = QLatin1String("opengl,direct3d,sdl");
        //    #else
        //            vo = QLatin1String("");
        //    #endif
        //        mpv_set_option_string(m_mpv, "vo", vo.toAscii().data());
    }


    mpv_set_option_string(m_mpv, "input-default-bindings", "yes");
    mpv_set_option_string(m_mpv, "input-vo-keyboard", "yes");
    mpv_set_option_string(m_mpv, "input-cursor", "no");
    mpv_set_option_string(m_mpv, "cursor-autohide", "no");

    mpv_observe_property(m_mpv, 0, "percent-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "pause", MPV_FORMAT_FLAG);

    mpv_request_log_messages(m_mpv, "error");  // no fatal error warn info v debug trace

    connect(this, SIGNAL(mpvEvents()), this, SLOT(receiveMpvEvents()), Qt::QueuedConnection);
    //    mpv_set_wakeup_callback(m_mpv, wakeup, this);
    qDebug() << "qvideowidget::mpv_initialize start!\n";
    if (mpv_initialize(m_mpv) < 0)
    {
        qDebug() << "MpvVideoPlayerBackend: mpv failed to initialize!\n";
        return false;
    }

    return true;
}

void MpvWidget::handleMpvEvent(mpv_event *event)
{
    switch (event->event_id)
    {
    case MPV_EVENT_PROPERTY_CHANGE:
    {
        mpv_event_property *prop = (mpv_event_property *)event->data;
        if (strcmp(prop->name, "percent-pos") == 0)
        {
            if (prop->format == MPV_FORMAT_DOUBLE)
            {
//                m_position = *(double *)prop->data;

                //                       if (m_state == Backward)
                //                           playBackward();
                //                       else if (m_state == Forward)
                //                           playForward();

                //                       emit currentPosition(m_position);
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
                //                       if (m_duration != duration)
                //                       {
                //                           m_duration = duration;

                //                           if (m_duration > 0)
                //                               mpv_set_option_string(m_mpv, "hr-seek", "yes");
                //                           durationIsKnown();
                //                       }
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
