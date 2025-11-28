#include "pdfviewer.h"


PDFViewer::PDFViewer(QWidget *parent) : QWidget(parent)
    , m_toolbar(new QToolBar(this))
    , m_statusbar(new QStatusBar(this))
    , m_returnBtn(new QPushButton(tr("Return"), this))
{
    m_pdfDocument = new QPdfDocument(this);
    setupUI();
    setupStyle();
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

    m_statusbar->showMessage(QString("已加载PDF: %1, 共 %2 页").arg(filePath).arg(m_totalPages));
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

    m_scaleLabel->setText(QString("缩放: %1%").arg(int(m_scaleFactor * 100)));


    // 显示页面
    for (int i = 0; i < pagesToShow; ++i) {
        QImage image = m_pdfDocument->render(i, QSize(m_scaleFactor * 200, m_scaleFactor * 300));

        QLabel *pageLabel = new QLabel();
        pageLabel->setPixmap(QPixmap::fromImage(image));
        pageLabel->setAlignment(Qt::AlignCenter);
        pageLabel->setStyleSheet("border: 2px solid #BBDEFB; margin: 10px; background: white; border-radius: 8px;");
        pageLabel->setFrameStyle(QFrame::Box);

        // 添加页码
        QLabel *pageNumberLabel = new QLabel(QString("第 %1 页").arg(i + 1));
        pageNumberLabel->setAlignment(Qt::AlignCenter);
        pageNumberLabel->setStyleSheet("color: #1976D2; margin-bottom: 5px; font-weight: bold;");

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
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建工具栏
    setupToolBar();
    mainLayout->addWidget(m_toolbar);

    // 创建控制面板
    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *rangeLabel = new QLabel("显示范围:");
    m_pageRangeCombo = new QComboBox();
    m_pageRangeCombo->addItem("前5页");
    m_pageRangeCombo->addItem("全部页面");

    m_scaleLabel = new QLabel(QString("缩放: %1%").arg(int(m_scaleFactor * 100)));

    controlLayout->addWidget(rangeLabel);
    controlLayout->addWidget(m_pageRangeCombo);
    controlLayout->addWidget(m_scaleLabel);
    controlLayout->addStretch();
    controlLayout->addWidget(m_returnBtn);

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
    m_statusbar->showMessage("就绪");
    mainLayout->addWidget(m_statusbar);
}

void PDFViewer::setupToolBar()
{
    QAction *openAction = new QAction("打开", this);
    QAction *zoomInAction = new QAction("放大", this);
    QAction *zoomOutAction = new QAction("缩小", this);

    m_toolbar->addAction(openAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(zoomInAction);
    m_toolbar->addAction(zoomOutAction);

    connect(openAction, &QAction::triggered, this, &PDFViewer::openFile);
    connect(zoomInAction, &QAction::triggered, this, &PDFViewer::zoomIn);
    connect(zoomOutAction, &QAction::triggered, this, &PDFViewer::zoomOut);
}

void PDFViewer::connectSignals()
{
    connect(m_pageRangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PDFViewer::updatePageDisplay);

    connect(m_returnBtn, &QPushButton::clicked, this, &PDFViewer::returnBtnClicked);

    // m_pageNavigation = new QPdfPageNavigation(this);
    // m_pageNavigation->setDocument(m_pdfDocument);
    // connect(m_pageNavigation, &QPdfPageNavigation::currentPageChanged,
    //         this, &PDFViewer::onPageChanged);
}

void PDFViewer::setupStyle()
{
    setStyleSheet(
        "QWidget {"
        "    background-color: #E3F2FD;"
        "}"
        ""
        "QToolBar {"
        "    background-color: #BBDEFB;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px;"
        "    spacing: 5px;"
        "}"
        ""
        "QToolBar QToolButton {"
        "    background-color: transparent;"
        "    border: 2px solid transparent;"
        "    border-radius: 6px;"
        "    padding: 8px 15px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: #1976D2;"
        "}"
        ""
        "QToolBar QToolButton:hover {"
        "    background-color: #90CAF9;"
        "    color: #0D47A1;"
        "}"
        ""
        "QComboBox {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    padding: 6px 12px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #333333;"
        "    min-width: 100px;"
        "}"
        "QComboBox:focus {"
        "    border: 2px solid #2196F3;"
        "    background-color: #F5FAFF;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    background-color: #2196F3;"
        "    border-radius: 0 8px 8px 0;"
        "    width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 6px solid white;"
        "    width: 0;"
        "    height: 0;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    selection-background-color: #2196F3;"
        "    selection-color: white;"
        "    background-color: white;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #2196F3;"
        "    padding: 8px 20px;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
        ""
        "QLabel {"
        "    color: #424242;"
        "    font-size: 14px;"
        "}"
        ""
        "QStatusBar {"
        "    background-color: #BBDEFB;"
        "    color: #1976D2;"
        "    border-top: 1px solid #90CAF9;"
        "}"
        ""
        "QScrollArea {"
        "    background-color: #E3F2FD;"
        "    border: none;"
        "}"
        ""
        "QScrollBar:vertical {"
        "    background-color: #BBDEFB;"
        "    width: 12px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #64B5F6;"
        "    border-radius: 6px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background-color: #42A5F5;"
        "}"
        ""
        "QScrollBar:horizontal {"
        "    background-color: #BBDEFB;"
        "    height: 12px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::handle:horizontal {"
        "    background-color: #64B5F6;"
        "    border-radius: 6px;"
        "    min-width: 20px;"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "    background-color: #42A5F5;"
        "}"
    );
}

