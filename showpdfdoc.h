#ifndef SHOWPDFDOC_H
#define SHOWPDFDOC_H

#include <QWidget>
//#include <QWebEngineView>
#include <QUrl>
#include <QPushButton>

class ShowPdfDoc : public QWidget
{
    Q_OBJECT
public:
    explicit ShowPdfDoc(QWidget *parent = nullptr);
    void showPdfDocument(QString name);
signals:

public slots:

private:
    //QWebEngineView *m_pdfView;
    QPushButton *m_closeBtn;
    QPushButton *m_prev;
    QPushButton *m_next;
};

#endif // SHOWPDFDOC_H
