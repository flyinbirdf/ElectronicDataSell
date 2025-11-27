#include "pdfviewer.h"


PDFViewer::PDFViewer(QWidget *parent) : QMainWindow(parent)
{
    m_pdfDocument = new QPdfDocument(this);
    setupUI();
    connectSignals();
}

void PDFViewer::loadPDF(const QString &filePath)
{
    if (filePath.isEmpty()) return;

    QPdfDocument::Error error = m_pdfDocument->load(filePath);
    if (error != QPdfDocument::Error::None) {
        QMessageBox::warning(this, "错误", "无法加载PDF文件");
        return;
    }

    m_totalPages = m_pdfDocument->pageCount();
    //m_pageNavigation->setPageCount(m_totalPages);
    //m_pageNavigation->setCurrentPage(0);

    statusBar()->showMessage(QString("已加载PDF: %1, 共 %2 页").arg(filePath).arg(m_totalPages));
    updatePageDisplay();
}

void PDFViewer::updatePageDisplay()
{
    // 清除现有内容
    QLayoutItem *item;
    while ((item = m_pagesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int pagesToShow = m_totalPages;
    if (m_pageRangeCombo->currentIndex() == 0) { // 前5页
        pagesToShow = qMin(5, m_totalPages);
    }

    // 显示页面
    for (int i = 0; i < pagesToShow; ++i) {
        QImage image = m_pdfDocument->render(i, QSize(m_scaleFactor * 200, m_scaleFactor * 300));

        QLabel *pageLabel = new QLabel();
        pageLabel->setPixmap(QPixmap::fromImage(image));
        pageLabel->setAlignment(Qt::AlignCenter);
        pageLabel->setStyleSheet("border: 1px solid #ccc; margin: 10px; background: white;");
        pageLabel->setFrameStyle(QFrame::Box);

        // 添加页码
        QLabel *pageNumberLabel = new QLabel(QString("第 %1 页").arg(i + 1));
        pageNumberLabel->setAlignment(Qt::AlignCenter);
        pageNumberLabel->setStyleSheet("color: #666; margin-bottom: 5px;");

        QVBoxLayout *pageLayout = new QVBoxLayout();
        pageLayout->addWidget(pageNumberLabel);
        pageLayout->addWidget(pageLabel);
        pageLayout->setAlignment(Qt::AlignCenter);

        QWidget *pageWidget = new QWidget();
        pageWidget->setLayout(pageLayout);

        m_pagesLayout->addWidget(pageWidget);
    }

    // 添加弹性空间
    m_pagesLayout->addStretch();
}

void PDFViewer::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "打开PDF文件", "", "PDF文件 (*.pdf)");

    if (!filePath.isEmpty()) {
        loadPDF(filePath);
    }
}

void PDFViewer::zoomIn()
{
    m_scaleFactor *= 1.2;
    updatePageDisplay();
}

void PDFViewer::zoomOut()
{
    m_scaleFactor /= 1.2;
    updatePageDisplay();
}

void PDFViewer::onPageChanged(int page)
{
    if (m_pageRangeCombo->currentIndex() == 0 && page >= 5) {
        // 如果当前显示前5页但用户导航到第5页之后，自动切换到全部页面
        m_pageRangeCombo->setCurrentIndex(1);
    }
}

void PDFViewer::setupUI()
{
    setWindowTitle("PDF查看器");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建工具栏
    setupToolBar();

    // 创建控制面板
    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *rangeLabel = new QLabel("显示范围:");
    m_pageRangeCombo = new QComboBox();
    m_pageRangeCombo->addItem("前5页");
    m_pageRangeCombo->addItem("全部页面");

    QLabel *scaleLabel = new QLabel(QString("缩放: %1%").arg(int(m_scaleFactor * 100)));

    controlLayout->addWidget(rangeLabel);
    controlLayout->addWidget(m_pageRangeCombo);
    controlLayout->addStretch();
    controlLayout->addWidget(scaleLabel);

    mainLayout->addLayout(controlLayout);

    // 创建滚动区域用于显示页面
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *pagesContainer = new QWidget();
    m_pagesLayout = new QVBoxLayout(pagesContainer);
    m_pagesLayout->setAlignment(Qt::AlignTop);
    m_pagesLayout->setSpacing(20);

    scrollArea->setWidget(pagesContainer);
    mainLayout->addWidget(scrollArea);

    // 创建状态栏
    statusBar()->showMessage("就绪");
}

void PDFViewer::setupToolBar()
{
    QToolBar *toolBar = addToolBar("主工具栏");

    QAction *openAction = new QAction("打开", this);
    QAction *zoomInAction = new QAction("放大", this);
    QAction *zoomOutAction = new QAction("缩小", this);

    toolBar->addAction(openAction);
    toolBar->addSeparator();
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);

    connect(openAction, &QAction::triggered, this, &PDFViewer::openFile);
    connect(zoomInAction, &QAction::triggered, this, &PDFViewer::zoomIn);
    connect(zoomOutAction, &QAction::triggered, this, &PDFViewer::zoomOut);
}

void PDFViewer::connectSignals()
{
    connect(m_pageRangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PDFViewer::updatePageDisplay);

    // m_pageNavigation = new QPdfPageNavigation(this);
    // m_pageNavigation->setDocument(m_pdfDocument);
    // connect(m_pageNavigation, &QPdfPageNavigation::currentPageChanged,
    //         this, &PDFViewer::onPageChanged);
}

