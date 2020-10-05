#include "mainwindow.hpp"

#include <future>
#include <thread>

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSettings>

#include "launchersettings.hpp"
#include "layouts/aboutdialog.hpp"
#include "layouts/launchersettingsdialog.hpp"

using namespace std::chrono_literals;

const auto Q_IP_REGEX =
    QRegExp( "^(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\.(?:[0-1]?[0-9]?[0-9]"
             "|2[0-4][0-9]|25[0-5])\\.(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])"
             "\\.(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$" );

CMainWindow::CMainWindow( LauncherSettings* settings, QWidget* pParent )
    : QMainWindow( pParent ), m_pSettings( settings )
{
    this->setupUi( this );
    this->ConnectActions();

    this->serverAddressInput->setValidator(
        new QRegExpValidator( Q_IP_REGEX, this ) );

    this->ParseSettings();
}

CMainWindow::~CMainWindow() {}

void CMainWindow::ParseSettings()
{
    this->m_pSettings->LoadFromFile();

    this->setGeometry( this->m_pSettings->GetMainWinRect() );
    this->usernameInput->setText( this->m_pSettings->GetUsername() );
    this->passwordInput->setText( this->m_pSettings->GetPassword() );
    this->serverAddressInput->setText( this->m_pSettings->GetServerIp() );
    this->serverPortInput->setText( this->m_pSettings->GetServerPort() );
}

void CMainWindow::SaveSettings() const
{
    this->m_pSettings->SetMainWinRect( this->geometry() );
    this->m_pSettings->SetUsername( this->usernameInput->text() );
    this->m_pSettings->SetPassword( this->passwordInput->text() );
    this->m_pSettings->SetServerIp( this->serverAddressInput->text() );
    this->m_pSettings->SetServerPort( this->serverPortInput->text() );

    this->m_pSettings->SaveToFile();
}

void CMainWindow::ConnectActions()
{
    this->connect( this->action_Quit, &QAction::triggered, this,
                   &CMainWindow::OnQuitButton );

    this->connect( this->usernameInput, &QLineEdit::textChanged, this,
                   &CMainWindow::OnInputChanged );
    this->connect( this->passwordInput, &QLineEdit::textChanged, this,
                   &CMainWindow::OnInputChanged );
    this->connect( this->serverAddressInput, &QLineEdit::textChanged, this,
                   &CMainWindow::OnInputChanged );
    this->connect( this->serverPortInput, &QLineEdit::textChanged, this,
                   &CMainWindow::OnInputChanged );
    this->connect( this->launchBtn, &QPushButton::clicked, this,
                   &CMainWindow::OnLaunchButton );

    this->connect( this->actionLauncher_arguments, &QAction::triggered, this,
                   &CMainWindow::OnLauncherArguments );
    this->connect( this->actionAbout_CSBS, &QAction::triggered, this,
                   &CMainWindow::OnAbout );
    this->connect( this->actionAbout_Qt, &QAction::triggered, this,
                   &QApplication::aboutQt );
}

void CMainWindow::ValidateInputs()
{
    const bool bCanSubmit =
        this->usernameInput->text().isEmpty() == false &&
        this->passwordInput->text().isEmpty() == false &&
        Q_IP_REGEX.exactMatch( this->serverAddressInput->text() ) == true &&
        this->serverPortInput->text().isEmpty() == false;

    if ( bCanSubmit == true )
    {
        this->m_pSettings->SaveToFile();
    }

    this->launchBtn->setEnabled( bCanSubmit );
}

void CMainWindow::OnInputChanged( const QString& /*changedText*/ )
{
    this->ValidateInputs();
}

std::pair<bool, QFileInfo> FindGameLauncher()
{
    const auto curDir = QDir::current();
    QFileInfo launcherInfo( curDir, QStringLiteral( "Bin/launcher.exe" ) );

    if ( launcherInfo.exists() == true )
    {
        return { true, std::move( launcherInfo ) };
    }

    launcherInfo.setFile( curDir, QStringLiteral( "launcher.exe" ) );

    if ( launcherInfo.exists() == true )
    {
        return { true, std::move( launcherInfo ) };
    }

    return { false, QFileInfo() };
}

void CMainWindow::OnLaunchButton()
{
    this->launchBtn->setEnabled( false );
    this->lblStatus->setText( tr( "Starting the game..." ) );

    this->SaveSettings();

    auto [launcherFound, launcherInfo] = FindGameLauncher();

    if ( launcherFound == false )
    {
        this->launchBtn->setEnabled( true );
        this->lblStatus->setText( tr( "Unable to find launcher.exe" ) );
        return;
    }

    if ( this->StartLauncher( launcherInfo ) == false )
    {
        this->launchBtn->setEnabled( true );
        this->lblStatus->setText( tr( "Failed to start launcher.exe" ) );
        return;
    }

    this->lblStatus->setText( tr( "Started the game successfully" ) );

    auto successTimeout = std::async(
        std::launch::async, [] { std::this_thread::sleep_for( 3000ms ); } );

    if ( successTimeout.valid() == true )
    {
        std::future_status status;

        do
        {
            status = successTimeout.wait_for( 5ms );
            QCoreApplication::processEvents();
        } while ( status != std::future_status::ready );
    }

    this->OnQuitButton();
}

bool CMainWindow::StartLauncher( const QFileInfo& launcherInfo ) const
{
    const QString username = this->usernameInput->text();
    const QString password = this->passwordInput->text();
    const QString serverIp = this->serverAddressInput->text();
    const QString serverPort = this->serverPortInput->text();

#ifdef _WIN32
    QString programPath = launcherInfo.absoluteFilePath();
    QStringList launchArgs = {
        "-username", username, "-password",   password,
        "-masterip", serverIp, "-masterport", serverPort
    };
#else
    QString programPath = QStringLiteral( "wine" );

    QStringList launchArgs = { launcherInfo.absoluteFilePath(),
                               "-username",
                               username,
                               "-password",
                               password,
                               "-masterip",
                               serverIp,
                               "-masterport",
                               serverPort };
#endif

    if ( this->m_pSettings->IsConsoleEnabled() == true )
    {
        launchArgs.append( QStringLiteral( "-console" ) );
    }

    if ( this->m_pSettings->ShouldUseUnpackedFiles() == true )
    {
        launchArgs.append( QStringLiteral( "-unpackedfiles" ) );
    }

    const QString& lang = this->m_pSettings->GetLanguageFile();

    if ( lang.isEmpty() == false )
    {
        launchArgs.append( { QStringLiteral( "-lang" ), lang } );
    }

    const QStringList& customArgs = this->m_pSettings->GetAdditionalArgs();

    if ( customArgs.isEmpty() == false )
    {
        launchArgs.append( customArgs );
    }

    const QString& customScript = this->m_pSettings->GetLaunchScript();

    if ( customScript.isEmpty() == false )
    {
        return QProcess::startDetached( customScript, launchArgs );
    }
    else
    {
        return QProcess::startDetached( programPath, launchArgs );
    }
}

void CMainWindow::OnQuitButton()
{
    this->SaveSettings();
    std::exit( 0 );
}

void CMainWindow::OnLauncherArguments()
{
    CLauncherSettingsDialog( this->m_pSettings, this ).exec();
}

void CMainWindow::OnAbout()
{
    CAboutDialog( this ).exec();
}
