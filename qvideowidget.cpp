#include "qvideowidget.h"
#include <QFrame>
#include <QKeyEvent>

static void wakeup(void *ptr)
{
    qvideowidget *backend = (qvideowidget*) ptr;
    backend->emitEvents();
}


qvideowidget::qvideowidget(QWidget *parent) : QWidget(parent)
{

    qDebug() << "qvideowidget: constructor!\n";

}

qvideowidget::~qvideowidget()
{
    qDebug() << "qvideowidget: deconstructor!\n";
}

void qvideowidget::emitEvents()
{
    if (m_mpv)
        emit mpvEvents();
}

void qvideowidget::setViewport(QWidget *viewport)
{
    if (m_viewport)
    {
        m_viewport->removeEventFilter(this);
        m_viewport->deleteLater();
    }

    m_viewport = viewport;
    //m_viewport->setAttribute(Qt::WA_DontCreateNativeAncestors);
    //m_viewport->setAttribute(Qt::WA_NativeWindow);
    m_viewport->setParent(this);
    m_viewport->setGeometry(contentsRect());
    m_viewport->setAutoFillBackground(false);
    m_viewport->setStyleSheet(QLatin1String("background-color:black;"));
    m_viewport->show();
}

void qvideowidget::setFullScreen(bool on)
{
    if (on)
    {
        setWindowFlags(windowFlags() | Qt::Window);
        //        m_normalFrameStyle = frameStyle();
        //        setFrameStyle(QWidget::NoFrame);
        showFullScreen();
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::Window);
        //        setFrameStyle(m_normalFrameStyle);
        showNormal();
    }
}

void qvideowidget::setOverlayMessage(const QString &message)
{
    if (message == m_overlayMsg)
        return;

    m_overlayMsg = message;
    m_viewport->update();
}

bool qvideowidget::start(const QUrl &url)
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
    qDebug() << "MpvVideoPlayerBackend: start create MPV \n";

    if (!createMpvProcess())
    {
        qDebug() << "MpvVideoPlayerBackend: start - failed to create MPV \n";
        return false;
    }



    m_playbackSpeed = 1.0;
    qDebug() << "MpvVideoPlayerBackend:: MPV process started\n";
    return true;
}

void qvideowidget::play()
{
    if (!m_mpv || m_state == Playing)
        return;

    int pause = 0;
    mpv_set_property(m_mpv, "pause", MPV_FORMAT_FLAG, &pause);

    //    emit playbackSpeedChanged(m_playbackSpeed);

    VideoState old = m_state;
    m_state = Playing;
    //    emit stateChanged(m_state, old);
}

void qvideowidget::setZoom(double z)
{
    m_zoomFactor = z;

    if (m_viewport)
    {
        int x, y, w, h;

        x = m_viewport->x();
        y = m_viewport->y();
        w = m_originalWidth;//m_viewport->width();
        h = m_originalHeight;//m_viewport->height();

        if (m_zoomFactor != 1.0)
        {
            w = w * m_zoomFactor;
            h = h * m_zoomFactor;

            x = (this->width() - w) / 2;
            y = (this->height() -h) / 2;
        }

        m_viewport->move(x, y);
        m_viewport->resize(w, h);
    }
}

bool qvideowidget::saveScreenshot(QString &file)
{
    if (!m_mpv)
        return false;

    const char *cmd[] = { "screenshot-to-file", file.toUtf8(), "video", NULL };
    mpv_command(m_mpv, cmd);

    return true;
}

void qvideowidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_viewport->setGeometry(contentsRect());

    int x, y, w, h;

    x = 0;
    y = 0;
    m_originalWidth = w = this->width();
    m_originalHeight = h = this->height();

    m_viewport->move(x, y);
    m_viewport->resize(w, h);

    setZoom(m_zoomFactor);

}

void qvideowidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->accept();
    toggleFullScreen();
}

void qvideowidget::keyPressEvent(QKeyEvent *event)
{

    if (event->modifiers() != 0)
        return;

    switch (event->key())
    {
    case Qt::Key_Escape:
        setFullScreen(false);
        break;
    default:
        return;
    }

    event->accept();

}

bool qvideowidget::createMpvProcess()
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
        //        QString vo = settings.value(QLatin1String("eventPlayer/mpv_vo"), QLatin1String("default")).toString();

        //        if (vo == QLatin1String("default"))
        //    #ifdef Q_OS_WIN
        //            vo = QLatin1String("opengl,direct3d,sdl");
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

void qvideowidget::handleMpvEvent(mpv_event *event)
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
                m_position = *(double *)prop->data;

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
        default: m_position = -1;
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
