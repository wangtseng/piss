#ifndef SURGERYLOADINGWINDOW_H
#define SURGERYLOADINGWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>

class SurgeryLoadingWindow : public QFrame
{
    Q_OBJECT

public:
    void initVariable();
    void constructIHM();
    void setConnections();

private:
    QHBoxLayout *surgeryLoadingWindowLayout;
    QLabel *titleBar;
    QWidget *advertisementArea;
    QLabel *loadingArea;

signals:

public slots:

public:
    explicit SurgeryLoadingWindow();
    ~SurgeryLoadingWindow();

};

#endif // SURGERYLOADINGWINDOW_H
