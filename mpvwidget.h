#ifndef MPVWIDGET_H
#define MPVWIDGET_H
#include <QObject>
#include <QWidget>
#include <QVariant>
#include <client.h>


class MpvWidget : public QWidget
{
    Q_OBJECT
public:
    enum State {STOPPED, VIDEO_PLAYING, VIDEO_PAUSED, TV_PLAYING};

    explicit MpvWidget(QWidget *parent = nullptr);
    ~MpvWidget();
    quint64 m_id;
    bool start(const QUrl &url);
    void audiomute();
    void audioon();
    void screenshot();
    void setVolume(int volume);

    int m_command_async(uint64_t reply_userdata,  const char **args);
    int m_set_property( const char *name, mpv_format format,void *data);
    int m_set_property_string(const char *name, const char *data);
    int m_set_property_async(uint64_t reply_userdata,
                               const char *name, mpv_format format, void *data);
    int m_get_property(const char *name, mpv_format format,
                         void *data);
    char *m_get_property_string( const char *name);

signals:
    void mpvEvents();
private slots:
    void receiveMpvEvents();

private:
    mpv_handle *m_mpv;
    State m_state = STOPPED;
    bool createMpvProcess();
    void handleMpvEvent(mpv_event *event);
    void showText(const QByteArray& text);
    void handleMpvError (int code);



};

#endif // MPVWIDGET_H
