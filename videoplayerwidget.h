#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QUrl>
#include <QStyle>

class VideoPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayerWidget(QWidget *parent = nullptr);
    ~VideoPlayerWidget();

    // 加载视频文件
    void loadVideo(const QString &filePath);
    // 设置视频URL
    void setVideoUrl(const QUrl &url);
    // 播放
    void play();
    // 暂停
    void pause();
    // 停止
    void stop();
    // 设置音量
    void setVolume(int volume);

signals:
    void videoFinished();
    void errorOccurred(const QString &errorString);

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onVolumeChanged(int volume);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderPositionChanged(int position);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayerError(QMediaPlayer::Error error, const QString &errorString);
    void onStateChanged(QMediaPlayer::PlaybackState state);

private:
    void setupUI();
    void setupStyle();
    void setupConnections();
    QString formatTime(qint64 timeInMs);

private:
    QMediaPlayer *m_mediaPlayer;
    QVideoWidget *m_videoWidget;
    QAudioOutput *m_audioOutput;
    
    // 控制按钮
    QPushButton *m_playPauseBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_volumeBtn;
    
    // 进度控制
    QSlider *m_positionSlider;
    QLabel *m_timeLabel;
    
    // 音量控制
    QSlider *m_volumeSlider;
    QLabel *m_volumeLabel;
    
    // 布局
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_controlLayout;
    QHBoxLayout *m_progressLayout;
    
    bool m_isPlaying;
    qint64 m_duration;
};

#endif // VIDEOPLAYERWIDGET_H

