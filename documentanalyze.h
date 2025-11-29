#ifndef DOCUMENTANALYZE_H
#define DOCUMENTANALYZE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QPdfDocument>
#include <QImage>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfoList>
#include <QPainter>
#include <QFont>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QVideoSink>
#endif

// 文档信息结构
struct DocumentInfo
{
    QString name;           // 文档名称
    QString description;   // 文档描述
    QString filePath;      // 文件路径
    QString thumbnailPath;  // 缩略图路径
    QString type;          // 类型：pdf/video
};

class DocumentAnalyze : public QObject
{
    Q_OBJECT
public:
    explicit DocumentAnalyze(QObject *parent = nullptr);
    ~DocumentAnalyze();

    // 设置销售路径
    void setSellPath(const QString &path);
    
    // 开始分析
    void startAnalyze();
    
    // 获取所有文档信息
    QList<DocumentInfo> getAllDocuments() const;
    
    // 获取所有视频信息
    QList<DocumentInfo> getAllVideos() const;
    
    // 从数据库查询文档信息
    QList<DocumentInfo> queryFromDatabase(const QString &type = "") const;

signals:
    void analyzeProgress(int current, int total, const QString &currentFile);
    void analyzeFinished();
    void errorOccurred(const QString &error);

private:
    // 扫描目录获取所有文档和视频
    void scanDirectory(const QString &path);
    
    // 处理PDF文档
    bool processPDF(const QString &filePath, DocumentInfo &info);
    
    // 处理视频文件
    bool processVideo(const QString &filePath, DocumentInfo &info);
    
    // 从PDF提取首页图片
    QImage extractPDFFirstPage(const QString &filePath);
    
    // 从视频提取首帧图片
    QImage extractVideoFirstFrame(const QString &filePath);
    
    // 保存图片到本地
    QString saveImage(const QImage &image, const QString &fileName);
    
    // 初始化向量数据库
    bool initVectorDatabase();
    
    // 将文档信息写入向量数据库
    bool insertToVectorDB(const DocumentInfo &info);
    
    // 生成文档描述（可以从文件名或内容提取）
    QString generateDescription(const QString &filePath, const QString &type);

private:
    QString m_sellPath;                    // 销售路径
    QList<DocumentInfo> m_documents;      // 文档列表
    QList<DocumentInfo> m_videos;         // 视频列表
    QSqlDatabase m_database;                // 向量数据库
    QString m_thumbnailDir;                // 缩略图存储目录
    QStringList m_supportedDocs;           // 支持的文档格式
    QStringList m_supportedVideos;         // 支持的视频格式
};

#endif // DOCUMENTANALYZE_H
