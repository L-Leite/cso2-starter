#pragma once

#include <QRect>
#include <QString>
#include <QStringList>

class LauncherSettings
{
public:
    LauncherSettings();
    ~LauncherSettings() = default;

public:
    void LoadFromFile();
    void SaveToFile() const;

    [[nodiscard]] inline const QRect& GetMainWinRect() const
    {
        return this->m_MainWindowRect;
    }

    void SetMainWinRect( const QRect& newRect )
    {
        this->m_MainWindowRect = newRect;
    }

    [[nodiscard]] inline const QString& GetUsername() const
    {
        return this->m_szUsername;
    }

    void SetUsername( QString&& newUsername )
    {
        this->m_szUsername = newUsername;
    }

    [[nodiscard]] inline const QString& GetPassword() const
    {
        return this->m_szPassword;
    }

    void SetPassword( QString&& newPassword )
    {
        this->m_szPassword = newPassword;
    }

    [[nodiscard]] inline const QString& GetServerIp() const
    {
        return this->m_szServerIp;
    }

    void SetServerIp( QString&& newIp ) { this->m_szServerIp = newIp; }

    [[nodiscard]] inline const QString& GetServerPort() const
    {
        return this->m_szServerPort;
    }

    void SetServerPort( QString&& newPort ) { this->m_szServerPort = newPort; }

    [[nodiscard]] inline const QStringList& GetAdditionalArgs() const
    {
        return this->m_vAdditionalArgs;
    }

    [[nodiscard]] inline const QString& GetLanguageFile() const
    {
        return this->m_szLanguageFile;
    }

    void SetLanguageFile( QString&& newLang )
    {
        this->m_szLanguageFile = newLang;
    }

    [[nodiscard]] inline const QString& GetLaunchScript() const
    {
        return this->m_szLaunchScript;
    }

    void SetLaunchScript( QString&& newScript )
    {
        this->m_szLaunchScript = newScript;
    }

    void SetAdditionalArgs( QStringList&& newArgs )
    {
        this->m_vAdditionalArgs = newArgs;
    }

    [[nodiscard]] inline bool IsConsoleEnabled() const
    {
        return this->m_bEnableConsole;
    }

    void SetConsoleEnabled( bool bEnabled )
    {
        this->m_bEnableConsole = bEnabled;
    }

    [[nodiscard]] inline bool ShouldUseUnpackedFiles() const
    {
        return this->m_bEnableConsole;
    }

    void SetUseUnpackedFiles( bool bUse ) { this->m_bUseUnpackedFiles = bUse; }

private:
    QRect m_MainWindowRect;

    QString m_szUsername;
    QString m_szPassword;
    QString m_szServerIp;
    QString m_szServerPort;

    QString m_szLanguageFile;
    QString m_szLaunchScript;
    QStringList m_vAdditionalArgs;
    bool m_bEnableConsole;
    bool m_bUseUnpackedFiles;
};
