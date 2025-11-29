#include "documentanalyze.h"
#include <QDirIterator>
#include <QPixmap>
#include <QStandardPaths>
#include <QApplication>
#include <QThread>
#include <QMediaMetaData>
#include <QTimer>
#include <QEventLoop>
#include <QVideoFrame>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QVideoSink>
#endif
#include <QMediaMetaData>

DocumentAnalyze::DocumentAnalyze(QObject *parent)
    : QObject{parent}
    , m_sellPath("")
{
    // 初始化支持的格式
    m_supportedDocs << "pdf" << "PDF";
    m_supportedVideos << "mp4" << "avi" << "mov" << "mkv" << "wmv" << "flv" 
                      << "MP4" << "AVI" << "MOV" << "MKV" << "WMV" << "FLV";
    
    // 设置缩略图存储目录
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_thumbnailDir = appDataDir + "/thumbnails";
    QDir dir;
    if (!dir.exists(m_thumbnailDir)) {
        dir.mkpath(m_thumbnailDir);
    }
    
    // 初始化数据库
    initVectorDatabase();
}

DocumentAnalyze::~DocumentAnalyze()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

void DocumentAnalyze::setSellPath(const QString &path)
{
    m_sellPath = path;
}

void DocumentAnalyze::startAnalyze()
{
    if (m_sellPath.isEmpty()) {
        emit errorOccurred("销售路径未设置");
        return;
    }
    
    QDir dir(m_sellPath);
    if (!dir.exists()) {
        emit errorOccurred(QString("路径不存在: %1").arg(m_sellPath));
        return;
    }
    
    m_documents.clear();
    m_videos.clear();
    
    // 扫描目录
    scanDirectory(m_sellPath);
    
    emit analyzeFinished();
}

void DocumentAnalyze::scanDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }
    
    QDirIterator it(path, QDirIterator::Subdirectories);
    QStringList allFiles;
    
    // 收集所有文件
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if (fileInfo.isFile()) {
            allFiles.append(fileInfo.absoluteFilePath());
        }
    }
    
    int total = allFiles.size();
    int current = 0;
    
    // 处理每个文件
    for (const QString &filePath : allFiles) {
        current++;
        QFileInfo fileInfo(filePath);
        QString suffix = fileInfo.suffix();
        
        emit analyzeProgress(current, total, fileInfo.fileName());
        
        DocumentInfo info;
        info.filePath = filePath;
        info.name = fileInfo.baseName();
        
        // 处理PDF文档
        if (m_supportedDocs.contains(suffix)) {
            if (processPDF(filePath, info)) {
                m_documents.append(info);
            }
        }
        // 处理视频文件
        else if (m_supportedVideos.contains(suffix)) {
            if (processVideo(filePath, info)) {
                m_videos.append(info);
            }
        }
    }
}

bool DocumentAnalyze::processPDF(const QString &filePath, DocumentInfo &info)
{
    try {
        // 提取首页图片
        QImage firstPageImage = extractPDFFirstPage(filePath);
        if (firstPageImage.isNull()) {
            qWarning() << "无法提取PDF首页图片:" << filePath;
            return false;
        }
        
        // 保存缩略图
        QString thumbnailName = QFileInfo(filePath).baseName() + "_thumb.png";
        info.thumbnailPath = saveImage(firstPageImage, thumbnailName);
        
        // 生成描述
        info.description = generateDescription(filePath, "pdf");
        info.type = "pdf";
        
        // 写入向量数据库
        insertToVectorDB(info);
        
        return true;
    } catch (...) {
        qWarning() << "处理PDF时发生错误:" << filePath;
        return false;
    }
}

bool DocumentAnalyze::processVideo(const QString &filePath, DocumentInfo &info)
{
    try {
        // 提取首帧图片
        QImage firstFrameImage = extractVideoFirstFrame(filePath);
        if (firstFrameImage.isNull()) {
            qWarning() << "无法提取视频首帧图片:" << filePath;
            return false;
        }
        
        // 保存缩略图
        QString thumbnailName = QFileInfo(filePath).baseName() + "_thumb.png";
        info.thumbnailPath = saveImage(firstFrameImage, thumbnailName);
        
        // 生成描述
        info.description = generateDescription(filePath, "video");
        info.type = "video";
        
        // 写入向量数据库
        insertToVectorDB(info);
        
        return true;
    } catch (...) {
        qWarning() << "处理视频时发生错误:" << filePath;
        return false;
    }
}

QImage DocumentAnalyze::extractPDFFirstPage(const QString &filePath)
{
    QPdfDocument pdfDoc;
    QPdfDocument::Error error = pdfDoc.load(filePath);
    
    if (error != QPdfDocument::Error::None) {
        qWarning() << "无法加载PDF文件:" << filePath << "错误:" << error;
        return QImage();
    }
    
    if (pdfDoc.pageCount() == 0) {
        qWarning() << "PDF文件没有页面:" << filePath;
        return QImage();
    }
    
    // 渲染第一页
    QSizeF pageSize = pdfDoc.pagePointSize(0);
    QSize renderSize = pageSize.toSize() * 2; // 提高分辨率
    
    QImage image = pdfDoc.render(0, renderSize);
    
    return image;
}

QImage DocumentAnalyze::extractVideoFirstFrame(const QString &filePath)
{
    QImage image;
    
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt 6: 使用QVideoSink提取视频首帧
    QMediaPlayer *player = new QMediaPlayer(this);
    QVideoSink *videoSink = new QVideoSink(this);
    player->setVideoOutput(videoSink);
    
    // 连接视频帧信号
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(5000); // 5秒超时
    
    bool frameReceived = false;
    
    connect(videoSink, &QVideoSink::videoFrameChanged, [&](const QVideoFrame &frame) {
        if (!frameReceived && frame.isValid()) {
            frameReceived = true;
            // 将QVideoFrame转换为QImage
            QImage frameImage = frame.toImage();
            if (!frameImage.isNull()) {
                image = frameImage;
            }
            loop.quit();
        }
    });
    
    connect(player, &QMediaPlayer::errorOccurred, [&](QMediaPlayer::Error error, const QString &errorString) {
        qWarning() << "视频播放器错误:" << errorString;
        loop.quit();
    });
    
    connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    
    // 加载视频
    player->setSource(QUrl::fromLocalFile(filePath));
    player->setPosition(0);
    player->play();
    
    timeoutTimer.start();
    loop.exec();
    
    player->stop();
    player->deleteLater();
    videoSink->deleteLater();
#endif
    
    // 如果Qt 6方法失败或使用Qt 5，使用QVideoWidget作为备选方案
    if (image.isNull()) {
        qDebug() << "使用QVideoWidget提取视频首帧:" << filePath;
        
        QMediaPlayer *player2 = new QMediaPlayer(this);
        QVideoWidget *videoWidget = new QVideoWidget();
        videoWidget->resize(320, 240);
        videoWidget->show(); // 需要显示才能渲染
        
        player2->setVideoOutput(videoWidget);
        
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        player2->setSource(QUrl::fromLocalFile(filePath));
#else
        player2->setMedia(QUrl::fromLocalFile(filePath));
#endif
        
        player2->setPosition(0);
        player2->play();
        
        // 等待视频加载
        QEventLoop loop2;
        QTimer timer2;
        timer2.setSingleShot(true);
        timer2.setInterval(2000); // 2秒
        
        connect(player2, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::BufferedMedia || status == QMediaPlayer::BufferingMedia) {
                timer2.stop();
                loop2.quit();
            }
        });
        
        connect(&timer2, &QTimer::timeout, &loop2, &QEventLoop::quit);
        timer2.start();
        loop2.exec();
        
        QThread::msleep(500); // 等待渲染
        
        QPixmap pixmap = videoWidget->grab();
        if (!pixmap.isNull()) {
            image = pixmap.toImage();
        }
        
        player2->stop();
        videoWidget->hide();
        player2->deleteLater();
        videoWidget->deleteLater();
    }
    
    // 如果仍然失败，返回占位图像
    if (image.isNull()) {
        qWarning() << "无法提取视频首帧，使用占位图像:" << filePath;
        image = QImage(320, 240, QImage::Format_RGB32);
        image.fill(Qt::lightGray);
        
        // 在图像上绘制文字
        QPainter painter(&image);
        painter.setPen(Qt::darkGray);
        painter.setFont(QFont("Arial", 16));
        painter.drawText(image.rect(), Qt::AlignCenter, "视频预览");
    }
    
    return image;
}

QString DocumentAnalyze::saveImage(const QImage &image, const QString &fileName)
{
    if (image.isNull()) {
        return QString();
    }
    
    // 调整图片大小（缩略图）
    QImage thumbnail = image.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    QString filePath = m_thumbnailDir + "/" + fileName;
    if (thumbnail.save(filePath, "PNG")) {
        return filePath;
    }
    
    qWarning() << "保存图片失败:" << filePath;
    return QString();
}

bool DocumentAnalyze::initVectorDatabase()
{
    // 使用SQLite作为向量数据库
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/vector_db.sqlite";
    
    m_database = QSqlDatabase::addDatabase("QSQLITE", "DocumentAnalyzeDB");
    m_database.setDatabaseName(dbPath);
    
    if (!m_database.open()) {
        qWarning() << "无法打开数据库:" << m_database.lastError().text();
        return false;
    }
    
    // 创建表
    QSqlQuery query(m_database);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS document_vectors ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "description TEXT,"
        "file_path TEXT NOT NULL UNIQUE,"
        "thumbnail_path TEXT,"
        "type TEXT NOT NULL,"
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );
    
    if (!query.exec()) {
        qWarning() << "创建表失败:" << query.lastError().text();
        return false;
    }
    
    // 创建索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_file_path ON document_vectors(file_path)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_type ON document_vectors(type)");
    
    return true;
}

bool DocumentAnalyze::insertToVectorDB(const DocumentInfo &info)
{
    if (!m_database.isOpen()) {
        return false;
    }
    
    QSqlQuery query(m_database);
    query.prepare(
        "INSERT OR REPLACE INTO document_vectors "
        "(name, description, file_path, thumbnail_path, type) "
        "VALUES (:name, :description, :file_path, :thumbnail_path, :type)"
    );
    
    query.bindValue(":name", info.name);
    query.bindValue(":description", info.description);
    query.bindValue(":file_path", info.filePath);
    query.bindValue(":thumbnail_path", info.thumbnailPath);
    query.bindValue(":type", info.type);
    
    if (!query.exec()) {
        qWarning() << "插入数据库失败:" << query.lastError().text();
        return false;
    }
    
    return true;
}

QString DocumentAnalyze::generateDescription(const QString &filePath, const QString &type)
{
    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.baseName();
    
    // 简单的描述生成：基于文件名
    // 可以后续扩展为从文件内容提取描述
    QString description = baseName;
    
    // 移除常见的文件命名模式（如下划线、连字符等）
    description.replace("_", " ");
    description.replace("-", " ");
    
    // 添加类型信息
    if (type == "pdf") {
        description += " - PDF文档";
    } else if (type == "video") {
        description += " - 视频文件";
    }
    
    return description;
}

QList<DocumentInfo> DocumentAnalyze::getAllDocuments() const
{
    return m_documents;
}

QList<DocumentInfo> DocumentAnalyze::getAllVideos() const
{
    return m_videos;
}

QList<DocumentInfo> DocumentAnalyze::queryFromDatabase(const QString &type) const
{
    QList<DocumentInfo> results;
    
    if (!m_database.isOpen()) {
        return results;
    }
    
    QSqlQuery query(m_database);
    QString sql = "SELECT name, description, file_path, thumbnail_path, type FROM document_vectors";
    
    if (!type.isEmpty()) {
        sql += " WHERE type = :type";
        query.prepare(sql);
        query.bindValue(":type", type);
    } else {
        query.prepare(sql);
    }
    
    if (!query.exec()) {
        qWarning() << "查询数据库失败:" << query.lastError().text();
        return results;
    }
    
    while (query.next()) {
        DocumentInfo info;
        info.name = query.value("name").toString();
        info.description = query.value("description").toString();
        info.filePath = query.value("file_path").toString();
        info.thumbnailPath = query.value("thumbnail_path").toString();
        info.type = query.value("type").toString();
        results.append(info);
    }
    
    return results;
}
