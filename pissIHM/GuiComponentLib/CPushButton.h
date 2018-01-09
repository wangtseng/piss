#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QWidget>
#include<QPushButton>
#include <QMouseEvent>
#include <QPoint>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
     CPushButton(QWidget *parent=NULL);
    ~CPushButton();
     void pressed();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void  enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void mouseMove(QPoint pos);
    void mouseLeftButtonClicked();
    void mouseRightButtonClicked();
    void mouseLeftButtonReleased();
    void mouseRightButtonReleased();
    void mouseHover();
    void mouseLeaved();
    void mouseDoubleClicked();

public slots:
};

#endif // CPUSHBUTTON_H
