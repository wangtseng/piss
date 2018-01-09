#ifndef DIGITALSURGERYTIME_H
#define DIGITALSURGERYTIME_H

#include <QWidget>
#include <QLCDNumber>

class digitalSurgeryTime : public QLCDNumber
{
    Q_OBJECT
public:
    explicit digitalSurgeryTime(QWidget *parent = 0);

signals:

public slots:
};

#endif // DIGITALSURGERYTIME_H
