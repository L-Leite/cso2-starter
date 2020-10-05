#pragma once

#include <QMainWindow>

#include "ui_mainwindow.h"

class LauncherSettings;
class QFileInfo;

class CMainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    CMainWindow( LauncherSettings* settings, QWidget* pParent = nullptr );
    ~CMainWindow();

private:
    void ParseSettings();
    void SaveSettings() const;

    bool StartLauncher( const QFileInfo& launcherInfo ) const;

    void ConnectActions();
    void ValidateInputs();
    void OnInputChanged( const QString& changedText );

private slots:
    void OnLaunchButton();
    void OnQuitButton();
    void OnLauncherArguments();
    void OnAbout();

private:
    LauncherSettings* m_pSettings;
};
