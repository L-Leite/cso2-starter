#include "csstarterapp.hpp"

#include <QDebug>
#include <QResource>

#include "starter_version.hpp"

CCSStarterApp::CCSStarterApp( int& argc, char** argv )
    : QApplication( argc, argv )
{
    this->SetupSettingsInfo();

    if ( CCSStarterApp::ShouldLoadOwnIcons() == true )
    {
        CCSStarterApp::LoadOwnIcons();
    }

    this->m_Settings.LoadFromFile();

    this->m_pMainWindow = std::make_unique<CMainWindow>( &this->m_Settings );
    this->m_pMainWindow->show();
}

void CCSStarterApp::SetupSettingsInfo()
{
    this->setOrganizationName( "Luís Leite" );
    this->setOrganizationDomain( "leite.xyz" );
    this->setApplicationName( "CSO2Starter" );
    this->setApplicationVersion( QStringLiteral( STARTER_VERSION ) );
}

bool CCSStarterApp::ShouldLoadOwnIcons()
{
    QString sampleIconName = QStringLiteral( "document-open" );
    return QIcon::hasThemeIcon( sampleIconName ) == false;
}

void CCSStarterApp::LoadOwnIcons()
{
    qDebug() << "Theme doesn't have the 'document-open' icon, loading our "
                "own icons";

    auto iconsResourceFilename = QStringLiteral( "/icons-breeze.rcc" );

    bool bIconRccLoaded = QResource::registerResource(
        QGuiApplication::applicationDirPath() + iconsResourceFilename );

    if ( bIconRccLoaded == false )
    {
        qDebug() << "Could not load fallback Breeze icons";
        return;
    }

    QIcon::setThemeName( QStringLiteral( "Breeze" ) );
}
