#ifndef COLORCHOOSEWINDOW_H
#define COLORCHOOSEWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPixmap>
#include <QBrush>
#include <QBitmap>
#include <QSlider>
#include <QPushButton>


class ColorChooseWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ColorChooseWindow(QDialog *parent = 0);
    ~ColorChooseWindow();
     bool colorChosen();
     int getRedValue();
     int getGreenValue();
     int getBlueValue();

private:
    QVBoxLayout* mainLayout;
    QLabel* indicationLabel;

    QFrame* colorConfigurationFrame;
    QHBoxLayout *colorConfigurationFrameLayout;
    QWidget *colorAjustmentArea;
    QGridLayout* colorAjustmentAreaLayout;
    QLabel* redLabel;
    QLabel* greenLabel;
    QLabel* blueLabel;

    QLabel* redValueLabel;
    QLabel* greenValueLabel;
    QLabel* blueValueLabel;

    QSlider* redSlider;
    QSlider* greenSlider;
    QSlider* blueSlider;

    QWidget*colorDisplayArea;
    QVBoxLayout* colorDisplayAreaLayout;
    QFrame* colorDisplayAreaIndication;
    QLabel* colorDisplayer;

    QFrame* confirmationFrame;
    QHBoxLayout *confirmationFrameLayout;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    QPixmap *pixmap;

    int redValue;
    int greenValue;
    int blueValue;
    bool isConfirmed;
    QString colorDescription;

    void drawBackground();
    void variableInitialisation();
    void constructIHM();
    void embellishIHM();
    void setConnections();
    void colorUpdate();

signals:


public slots:
    void setRedValue(int redValue);
    void setGreenValue(int greenValue);
    void setBlueValue(int blueValue);
    void confirmed();
    void cancelled();
};

#endif // COLORCHOOSEWINDOW_H
