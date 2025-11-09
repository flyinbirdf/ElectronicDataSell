#include "showpdfdoc.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <poppler-qt5.h>
ShowPdfDoc::ShowPdfDoc(QWidget *parent) : QWidget(parent)
  //, m_pdfView(new QWebEngineView(this))
  , m_closeBtn(new QPushButton(tr("CLOSE"), this))
  , m_prev(new QPushButton(tr("PREV"), this))
  , m_next(new QPushButton(tr("NEXT"), this))
{
    QVBoxLayout *layout = new QVBoxLayout();

    //m_pdfView->setFixedSize(800, 400);
    //layout->addWidget(m_pdfView);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_prev);
    hlayout->addWidget(m_closeBtn);
    hlayout->addWidget(m_next);

    layout->addLayout(hlayout);

    this->setLayout(layout);
}

void ShowPdfDoc::showPdfDocument(QString)
{
    QString strDoc="D:/test.pdf";
    //QString strDoc="test.pdf";
    QUrl url = QUrl::fromLocalFile(strDoc);
    //m_pdfView->load(url);
    //m_pdfView->load(QUrl::fromLocalFile(strDoc));
    //m_pdfView->show();
    //Poppler::Document *document = Poppler::Document::load(strDoc);
}
