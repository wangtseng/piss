#ifndef PERSONNELIMAGELABEL_H
#define PERSONNELIMAGELABEL_H

#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>

class PersonnelImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PersonnelImageLabel( );
    ~PersonnelImageLabel();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);

private:
    QString imgPath;

public:
    QString getImagePath();

signals:

public slots:
};

#endif // PERSONNELIMAGELABEL_H
