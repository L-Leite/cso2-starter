#pragma once

#include <QDialog>
#include "ui_launchersettingsdialog.h"

class LauncherSettings;

class CLauncherSettingsDialog : public QDialog,
                                public Ui::LauncherSettingsDialog
{
    Q_OBJECT

public:
    CLauncherSettingsDialog( LauncherSettings* settings,
                             QWidget* pParent = nullptr );

private:
    void ConnectActions();

    void LoadSettings();
    void SaveSettings() const;

private slots:
    void OnAccepted();
    void OnScriptBrowseBtn();

private:
    LauncherSettings* m_pSettings;
};
