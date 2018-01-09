#ifndef ALGORITHMTESTPLATFORM_H
#define ALGORITHMTESTPLATFORM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QFont>
#include <QProgressBar>
#include <SystemDispatcher.h>
#include <Windows.h>
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <QTimer>

// Use to convert bytes to KB
#define DIV 1024

// Specify the width of the field in which to print the numbers.
// The asterisk in the format specifier "%*I64d" takes an integer
// argument and uses it to pad and right justify the number.
#define WIDTH 7


class AlgorithmTestPlatform : public QWidget
{
    Q_OBJECT
public:
    explicit AlgorithmTestPlatform(SystemDispatcher* dispatcher, int appWidth, int appHeight, QFont *caracterStyle);
    void setBackgroundColor(QString color);
    void setSystemStatus(QString status);
    void getPatientsStatus();
    void initVariable();
    void constructIHM();
    void setConnections();

    float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
    unsigned long long FileTimeToInt64(const FILETIME & ft);
    float GetCPULoad();

private:
    //SYSTEM_INFO
    QHBoxLayout* platformLayout;
    QVBoxLayout* terminalWindowLayout;
    QGridLayout* systemUsageStatusLayout;

    QTextEdit* systemStatus;
    QLineEdit* systemCommand;
    SystemDispatcher* dispatcher;

    QFont* caracterStyle;

    QLabel* systemUsageStatusWidget;
    QWidget* terminalWindow;

    QLabel* cpuLabel;
    QLabel* ramLabel;
    QLabel* diskLable;
    QLabel* netLable;
    QLabel *algorithmTestTitleLabel;
    QLabel *algorithmTestCommandLabel;


    QProgressBar* cpuProgressBar;
    QProgressBar* ramProgressBar;
    QProgressBar* diskProgressBar;
    QProgressBar* netProgressBar;

    QString labelStyleSheet;
    QString progressBarStyleSheet;

    QString systemMessage;

    int appWidth;
    int appHeight;

signals:
private slots:
    void doParseCommand();
    void getCpuUsage();
};

#endif // ALGORITHMTESTPLATFORM_H
