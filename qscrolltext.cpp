#include "qscrolltext.h"
#include <QPainter>
#include <QDebug>

QScrollText::QScrollText(QWidget *parent) :
    QWidget(parent), scrollPos(0)
{
    staticText.setTextFormat(Qt::PlainText);
    setFixedHeight(fontMetrics().height());
    leftMargin = width();
    setSeparator("   ");
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    timer.setInterval(50);
}

QString QScrollText::text() const
{
    return _text;
}

void QScrollText::setText(QString text)
{
    _text = text;
    updateText();
    update();
}

QString QScrollText::separator() const
{
    return _separator;
}

void QScrollText::setSeparator(QString separator)
{
    _separator = separator;
    updateText();
    update();
}
void QScrollText::mousePressEvent(QMouseEvent *event)
{
    if(wholeTextSize.width() < width())
        return;

    scrollPos = 0;
    if(timer.isActive())
    {
        timer.stop();
    }
    else
    {
        timer.start();
    }

    update();
}

void QScrollText::updateText()
{
    timer.stop();
    singleTextWidth =  fontMetrics().width(_text);
    //timer.start();
    scrollPos = 0;
    staticText.setText(_text + _separator);
    staticText.prepare(QTransform(), font());
    wholeTextSize = QSize(fontMetrics().width(staticText.text()), fontMetrics().height());
}

void QScrollText::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);

    buffer.fill(qRgba(0, 0 ,0, 0));
    QPainter pb(&buffer);
    pb.setPen(painter.pen());
    pb.setFont(painter.font());

    int x = qMin(-scrollPos, 0);

    if(x < width())
    {
        pb.drawStaticText(QPointF(x, (height() - wholeTextSize.height()) / 2) + QPoint(0,0), staticText);
        x +=  wholeTextSize.width();

        if(x < 0)
        {
            scrollPos = 0;
            timer.stop();
            update();
        }

        painter.drawImage(0, 0, buffer);
    }

}

void QScrollText::timer_timeout()
{
    scrollPos = (scrollPos+ 2);
    update();
}
