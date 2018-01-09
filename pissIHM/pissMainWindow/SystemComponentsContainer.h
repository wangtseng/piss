#ifndef SYSTEMCOMPONENTSCONTAINER_H
#define SYSTEMCOMPONENTSCONTAINER_H

#include <QFrame>

/**
 * @brief The SystemComponentsContainer class
 * @author luo jian ming
 */
class SystemComponentsContainer : public QFrame
{
    Q_OBJECT

private:
    int width;
    int height;

public:
    explicit SystemComponentsContainer(int width, int height);

signals:

public slots:
};

#endif // SYSTEMCOMPONENTSCONTAINER_H
