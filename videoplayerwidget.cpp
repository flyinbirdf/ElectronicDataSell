#include "videoplayerwidget.h"
#include <QDebug>

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent)
    : QWidget(parent)
    , m_mediaPlayer(nullptr)
    , m_videoWidget(nullptr)
    , m_audioOutput(nullptr)
    , m_playPauseBtn(nullptr)
    , m_stopBtn(nullptr)
    , m_volumeBtn(nullptr)
    , m_positionSlider(nullptr)
    , m_timeLabel(nullptr)
    , m_volumeSlider(nullptr)
    , m_volumeLabel(nullptr)
    , m_mainLayout(nullptr)
    , m_controlLayout(nullptr)
    , m_progressLayout(nullptr)
    , m_isPlaying(false)
    , m_duration(0)
{
    setupUI();
    setupStyle();
    setupConnections();
}

VideoPlayerWidget::~VideoPlayerWidget()
{
    if (m_mediaPlayer) {
        m_mediaPlayer->stop();
    }
}

void VideoPlayerWidget::setupUI()
{
    // 创建音频输出
    m_audioOutput = new QAudioOutput(this);
    
    // 创建媒体播放器
    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    
    // 创建视频显示窗口
    m_videoWidget = new QVideoWidget(this);
    m_videoWidget->setMinimumSize(640, 360);
    m_mediaPlayer->setVideoOutput(m_videoWidget);
    
    // 创建播放/暂停按钮
    m_playPauseBtn = new QPushButton(this);
    m_playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_playPauseBtn->setToolTip(tr("播放/暂停"));
    m_playPauseBtn->setFixedSize(40, 40);
    
    // 创建停止按钮
    m_stopBtn = new QPushButton(this);
    m_stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopBtn->setToolTip(tr("停止"));
    m_stopBtn->setFixedSize(40, 40);
    
    // 创建音量按钮
    m_volumeBtn = new QPushButton(this);
    m_volumeBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    m_volumeBtn->setToolTip(tr("音量"));
    m_volumeBtn->setFixedSize(40, 40);
    
    // 创建进度条
    m_positionSlider = new QSlider(Qt::Horizontal, this);
    m_positionSlider->setRange(0, 0);
    m_positionSlider->setToolTip(tr("播放进度"));
    
    // 创建时间标签
    m_timeLabel = new QLabel("00:00 / 00:00", this);
    m_timeLabel->setMinimumWidth(120);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    
    // 创建音量滑块
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    m_volumeSlider->setMaximumWidth(100);
    m_volumeSlider->setToolTip(tr("音量"));
    
    // 创建音量标签
    m_volumeLabel = new QLabel("50%", this);
    m_volumeLabel->setMinimumWidth(40);
    m_volumeLabel->setAlignment(Qt::AlignCenter);
    
    // 创建进度布局
    m_progressLayout = new QHBoxLayout();
    m_progressLayout->setSpacing(10);
    m_progressLayout->addWidget(m_positionSlider, 1);
    m_progressLayout->addWidget(m_timeLabel);
    
    // 创建控制按钮布局
    m_controlLayout = new QHBoxLayout();
    m_controlLayout->setSpacing(10);
    m_controlLayout->addWidget(m_playPauseBtn);
    m_controlLayout->addWidget(m_stopBtn);
    m_controlLayout->addWidget(m_volumeBtn);
    m_controlLayout->addWidget(m_volumeSlider);
    m_controlLayout->addWidget(m_volumeLabel);
    m_controlLayout->addStretch();
    
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->addWidget(m_videoWidget, 1);
    m_mainLayout->addLayout(m_progressLayout);
    m_mainLayout->addLayout(m_controlLayout);
    
    setLayout(m_mainLayout);
    
    // 初始化音量（Qt 6中音量范围是0.0-1.0，50%对应0.5）
    m_audioOutput->setVolume(0.5);
}

void VideoPlayerWidget::setupStyle()
{
    setStyleSheet(
        "QWidget {"
        "    background-color: #1e1e1e;"
        "}"
        ""
        "QVideoWidget {"
        "    background-color: #000000;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 5px;"
        "    background-color: #2d2d2d;"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: #3d3d3d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1d1d1d;"
        "}"
        ""
        "QSlider::groove:horizontal {"
        "    border: 1px solid #555555;"
        "    height: 6px;"
        "    background: #2d2d2d;"
        "    border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #4CAF50;"
        "    border: 1px solid #4CAF50;"
        "    width: 14px;"
        "    margin: -4px 0;"
        "    border-radius: 7px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background: #66BB6A;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #4CAF50;"
        "    border-radius: 3px;"
        "}"
        ""
        "QLabel {"
        "    color: white;"
        "    font-size: 12px;"
        "}"
    );
}

void VideoPlayerWidget::setupConnections()
{
    // 按钮信号连接
    connect(m_playPauseBtn, &QPushButton::clicked, this, &VideoPlayerWidget::onPlayPauseClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &VideoPlayerWidget::onStopClicked);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &VideoPlayerWidget::onVolumeChanged);
    
    // 进度条信号连接
    connect(m_positionSlider, &QSlider::sliderPressed, this, [this]() {
        m_mediaPlayer->pause();
    });
    connect(m_positionSlider, &QSlider::sliderReleased, this, [this]() {
        if (m_isPlaying) {
            m_mediaPlayer->play();
        }
    });
    connect(m_positionSlider, &QSlider::valueChanged, this, &VideoPlayerWidget::onSliderPositionChanged);
    
    // 媒体播放器信号连接
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayerWidget::onPositionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayerWidget::onDurationChanged);
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoPlayerWidget::onMediaStatusChanged);
    connect(m_mediaPlayer, &QMediaPlayer::errorOccurred, 
            this, &VideoPlayerWidget::onPlayerError);
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoPlayerWidget::onStateChanged);
}

void VideoPlayerWidget::loadVideo(const QString &filePath)
{
    if (filePath.isEmpty()) {
        return;
    }
    
    m_mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
    m_mediaPlayer->play();
    m_isPlaying = true;
}

void VideoPlayerWidget::setVideoUrl(const QUrl &url)
{
    if (url.isEmpty()) {
        return;
    }
    
    m_mediaPlayer->setSource(url);
}

void VideoPlayerWidget::play()
{
    m_mediaPlayer->play();
    m_isPlaying = true;
}

void VideoPlayerWidget::pause()
{
    m_mediaPlayer->pause();
    m_isPlaying = false;
}

void VideoPlayerWidget::stop()
{
    m_mediaPlayer->stop();
    m_isPlaying = false;
    m_positionSlider->setValue(0);
}

void VideoPlayerWidget::setVolume(int volume)
{
    m_volumeSlider->setValue(volume);
    // Qt 6中音量范围是0.0-1.0，需要将0-100转换为0.0-1.0
    m_audioOutput->setVolume(volume / 100.0);
}

void VideoPlayerWidget::onPlayPauseClicked()
{
    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        pause();
    } else {
        play();
    }
}

void VideoPlayerWidget::onStopClicked()
{
    stop();
}

void VideoPlayerWidget::onVolumeChanged(int volume)
{
    // Qt 6中音量范围是0.0-1.0，需要将0-100转换为0.0-1.0
    m_audioOutput->setVolume(volume / 100.0);
    m_volumeLabel->setText(QString("%1%").arg(volume));
    
    // 更新音量按钮图标
    if (volume == 0) {
        m_volumeBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    } else if (volume < 50) {
        m_volumeBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    } else {
        m_volumeBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}

void VideoPlayerWidget::onPositionChanged(qint64 position)
{
    if (!m_positionSlider->isSliderDown()) {
        m_positionSlider->setValue(static_cast<int>(position));
    }
    
    m_timeLabel->setText(QString("%1 / %2")
                         .arg(formatTime(position))
                         .arg(formatTime(m_duration)));
}

void VideoPlayerWidget::onDurationChanged(qint64 duration)
{
    m_duration = duration;
    m_positionSlider->setRange(0, static_cast<int>(duration));
    m_timeLabel->setText(QString("%1 / %2")
                         .arg(formatTime(0))
                         .arg(formatTime(duration)));
}

void VideoPlayerWidget::onSliderPositionChanged(int position)
{
    if (m_positionSlider->isSliderDown()) {
        m_mediaPlayer->setPosition(static_cast<qint64>(position));
    }
}

void VideoPlayerWidget::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        emit videoFinished();
        m_isPlaying = false;
    }
}

void VideoPlayerWidget::onPlayerError(QMediaPlayer::Error error, const QString &errorString)
{
    QString errorMsg;
    if (!errorString.isEmpty()) {
        errorMsg = errorString;
    } else {
        switch (error) {
        case QMediaPlayer::ResourceError:
            errorMsg = tr("资源错误：无法加载视频文件");
            break;
        case QMediaPlayer::FormatError:
            errorMsg = tr("格式错误：不支持的视频格式");
            break;
        case QMediaPlayer::NetworkError:
            errorMsg = tr("网络错误：无法连接到视频源");
            break;
        case QMediaPlayer::AccessDeniedError:
            errorMsg = tr("访问被拒绝：没有权限访问视频文件");
            break;
        default:
            errorMsg = tr("未知错误：播放视频时发生错误");
            break;
        }
    }
    
    qDebug() << "VideoPlayer Error:" << errorMsg;
    emit errorOccurred(errorMsg);
}

void VideoPlayerWidget::onStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        m_playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        m_isPlaying = true;
        break;
    case QMediaPlayer::PausedState:
        m_playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        m_isPlaying = false;
        break;
    case QMediaPlayer::StoppedState:
        m_playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        m_isPlaying = false;
        break;
    }
}

QString VideoPlayerWidget::formatTime(qint64 timeInMs)
{
    int seconds = static_cast<int>(timeInMs / 1000);
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int hours = minutes / 60;
    minutes = minutes % 60;
    
    if (hours > 0) {
        return QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    }
}

