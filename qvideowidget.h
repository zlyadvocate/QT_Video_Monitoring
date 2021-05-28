#ifndef QVIDEOWIDGET_H
#define QVIDEOWIDGET_H

#include <QObject>
#include <QWidget>

// MVP库头文件
#include <QJsonDocument>
//#include <qthelper.hpp> // 该文件在最新的libmpv中已被弃用
#include <client.h>

enum VideoState
{
    PermanentError = -2, /* Permanent errors; i.e., playback will not work even if restarted */
    Error = -1, /* Recoverable errors, generally by stopping and restarting the pipeline */
    Stopped,
    Playing,
    Forward,
    Backward,
    Paused,
    Done
};



class qvideowidget : public QWidget
{
    Q_OBJECT
public:
    explicit qvideowidget(QWidget *parent = 0);
     ~qvideowidget();
    QString rtspaddr;
    quint64 m_id;
     void emitEvents();
     void setViewport(QWidget *viewport);



signals:
       void mpvEvents();
public slots:
    void setFullScreen(bool on);
    void toggleFullScreen() { setFullScreen(!isFullScreen()); }
    void setOverlayMessage(const QString &message);
    void clearOverlayMessage() { setOverlayMessage(QString()); }
    bool start(const QUrl &url);
//    void clear();
    void play();
//    void playIfReady();
//    void pause();
//    bool seek(int position);
//    bool setSpeed(double speed);
//    void restart();
//    void playForward();
//    void playBackward();
//    void mute(bool mute);
//    void setVolume(double volume);
//    void setBrightness(int brighness);
//    void setContrast(int contrast);
//    void setColor(int balance);
    void setZoom(double z);
    bool saveScreenshot(QString &file);

protected:
    void resizeEvent(QResizeEvent *event) ;
    void mouseDoubleClickEvent(QMouseEvent *event) ;
    void keyPressEvent(QKeyEvent *event) ;
private:
    QWidget *m_viewport;
    QString m_overlayMsg;
    int m_frameWidth, m_frameHeight;
    int m_normalFrameStyle;
    double m_zoomFactor;
    int m_originalWidth;
    int m_originalHeight;
    double m_position;
    double m_playbackSpeed;
    VideoState m_state;

    mpv_handle *m_mpv;



    bool createMpvProcess();
    void handleMpvEvent(mpv_event *event);

};

#endif // QVIDEOWIDGET_H
