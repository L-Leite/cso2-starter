#pragma once

#include <memory>

#include <QApplication>

#include "launchersettings.hpp"
#include "layouts/mainwindow.hpp"

class CCSStarterApp : public QApplication
{
    Q_OBJECT

public:
    CCSStarterApp( int& argc, char** argv );
    ~CCSStarterApp() = default;

private:
    void SetupSettingsInfo();

    static bool ShouldLoadOwnIcons();
    static void LoadOwnIcons();

private:
    std::unique_ptr<CMainWindow> m_pMainWindow;
    LauncherSettings m_Settings;
};
