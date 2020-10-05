#include "launchersettings.hpp"

#include <QSettings>

LauncherSettings::LauncherSettings()
    : m_MainWindowRect(), m_szUsername(), m_szPassword(), m_szServerIp(),
      m_szServerPort(), m_szLanguageFile(), m_szLaunchScript(),
      m_vAdditionalArgs(), m_bEnableConsole( false ),
      m_bUseUnpackedFiles( false )
{
}

void LauncherSettings::LoadFromFile()
{
    QSettings settings;
    settings.setDefaultFormat( QSettings::IniFormat );

    settings.beginGroup( QStringLiteral( "mainwindow" ) );
    this->m_MainWindowRect =
        settings.value( QStringLiteral( "geometry" ) ).value<QRect>();
    settings.endGroup();

    settings.beginGroup( QStringLiteral( "credentials" ) );
    this->m_szUsername =
        settings.value( QStringLiteral( "username" ) ).toString();
    this->m_szPassword =
        settings.value( QStringLiteral( "secret" ) ).toString();
    this->m_szServerIp =
        settings.value( QStringLiteral( "serverip" ) ).toString();
    this->m_szServerPort =
        settings.value( QStringLiteral( "serverport" ) ).toString();
    settings.endGroup();

    settings.beginGroup( QStringLiteral( "launcher" ) );
    this->m_bEnableConsole =
        settings.value( QStringLiteral( "console" ), false ).toBool();
    this->m_bUseUnpackedFiles =
        settings.value( QStringLiteral( "unpackedfiles" ), false ).toBool();
    this->m_szLanguageFile =
        settings.value( QStringLiteral( "langfile" ) ).toString();
    this->m_szLaunchScript =
        settings.value( QStringLiteral( "launchscript" ) ).toString();
    this->m_vAdditionalArgs =
        settings.value( QStringLiteral( "customargs" ) ).toStringList();
    settings.endGroup();
}

void LauncherSettings::SaveToFile() const
{
    QSettings settings;
    settings.setDefaultFormat( QSettings::IniFormat );

    settings.beginGroup( QStringLiteral( "mainwindow" ) );
    settings.setValue( QStringLiteral( "geometry" ), this->m_MainWindowRect );
    settings.endGroup();

    // FIXME: don't use password for credentials or save it in a better way
    settings.beginGroup( QStringLiteral( "credentials" ) );
    settings.setValue( QStringLiteral( "username" ), this->m_szUsername );
    settings.setValue( QStringLiteral( "secret" ), this->m_szPassword );
    settings.setValue( QStringLiteral( "serverip" ), this->m_szServerIp );
    settings.setValue( QStringLiteral( "serverport" ), this->m_szServerPort );
    settings.endGroup();

    settings.beginGroup( QStringLiteral( "launcher" ) );
    settings.setValue( QStringLiteral( "console" ), this->m_bEnableConsole );
    settings.setValue( QStringLiteral( "unpackedfiles" ),
                       this->m_bUseUnpackedFiles );
    settings.setValue( QStringLiteral( "langfile" ), this->m_szLanguageFile );
    settings.setValue( QStringLiteral( "launchscript" ),
                       this->m_szLaunchScript );
    settings.setValue( QStringLiteral( "customargs" ),
                       this->m_vAdditionalArgs );
    settings.endGroup();
}
