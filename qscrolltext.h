/*
 * Modified from the original source.
 * http://stackoverflow.com/questions/12159443/qt-scrolling-the-text-in-qwidget
 */
#ifndef QSCROLLTEXT_H
#define QSCROLLTEXT_H

#include <QWidget>
#include <QMouseEvent>
#include <QStaticText>
#include <QTimer>

class QScrollText : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString separator READ separator WRITE setSeparator)

public:
    explicit QScrollText(QWidget *parent = 0);

public slots:
    QString text() const;
    void setText(QString text);

    QString separator() const;
    void setSeparator(QString separator);

protected:
    void paintEvent(QPaintEvent *);

private:
    void mousePressEvent(QMouseEvent *event);
    void updateText();
    QString _text;
    QString _separator;
    QStaticText staticText;
    int singleTextWidth;
    QSize wholeTextSize;
    int leftMargin;
    int scrollPos;
    QImage buffer;
    QTimer timer;

private slots:
    virtual void timer_timeout();
};

#endif // QSCROLLTEXT_H
