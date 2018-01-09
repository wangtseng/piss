#ifndef SYSTEMOPTIONS_H
#define SYSTEMOPTIONS_H

#include <QFrame>
#include <QGridLayout>
#include <QComboBox>
#include <QColor>
#include <QPushButton>
#include <QLabel>


class SystemOptions : public QFrame
{
    Q_OBJECT

private:
    QGridLayout *systemOptionsLayout;

    QComboBox *systemBackgroundColorOptions;
    QComboBox *systemWorkSpaceColorOptions;

    QLabel *systemBackgroundColorOptionsLabel;
    QLabel *systemWorkSpaceColorOptionsLabel;

    QPushButton *confirmButton;
    QPushButton * cancelButton;

    QString comboBoxStyle;

    QString systemGlobalBackgroundColor;
    QString systemGlobalWorkSpaceColor;

public:
    void initVariable();
    void constructIHM();
    void setConnections();
    void fillColorList(QComboBox *systemBackgroundColorOptions);
    QString getSystemGlobalBackgroundColor();
    QString getSystemGlobalWorkSpaceColor();

signals:
    void confirm();
    void cancel();

public slots:
    void onConfirmButtonClicked();
    void onCancelButtonClicked();
    void systemColorOptionChanged();
    void workspaceColorOptionChanged();

public:
    SystemOptions();
};

#endif // SYSTEMOPTIONS_H
