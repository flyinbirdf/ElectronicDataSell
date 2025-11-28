#pragma once
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QScrollBar>
#include <QPdfDocument>
#include <QToolBar>
#include <QPdfView>
//#include <QPdfPageNavigation>

class PDFViewer : public QWidget
{
    Q_OBJECT

public:
    PDFViewer(QWidget *parent = nullptr);
    void loadPDF(const QString &filePath);

signals:
    void returnBtnClicked();

private slots:
    void updatePageDisplay();

    void openFile();

    void zoomIn();

    void zoomOut();

    void onPageChanged(int page);

private:
    void setupUI();

    void setupToolBar();

    void connectSignals();

private:
    QToolBar *m_toolbar;
    QStatusBar *m_statusbar;
    QPdfDocument *m_pdfDocument;
    QPdfView *m_pdfView;
    //QPdfPageNavigation *m_pageNavigation;
    QComboBox *m_pageRangeCombo = nullptr;
    QVBoxLayout *m_pagesLayout = nullptr;
    int m_totalPages = 0;
    qreal m_scaleFactor = 2.0; // 缩放因子
    QLabel *m_scaleLabel;
    QPushButton *m_returnBtn;
};

