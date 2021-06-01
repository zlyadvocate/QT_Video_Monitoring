#ifndef MPVWIDGET_H
#define MPVWIDGET_H
#include <QObject>
#include <QWidget>
#include <client.h>


class MpvWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MpvWidget(QWidget *parent = nullptr);
    ~MpvWidget();
    quint64 m_id;
    bool start(const QUrl &url);


signals:
    void mpvEvents();
private slots:
    void receiveMpvEvents();

private:
    mpv_handle *m_mpv;
    bool createMpvProcess();
    void handleMpvEvent(mpv_event *event);
};

#endif // MPVWIDGET_H
