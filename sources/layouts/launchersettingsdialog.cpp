#include "launchersettingsdialog.hpp"

#include "launchersettings.hpp"

#include <QFileDialog>

CLauncherSettingsDialog::CLauncherSettingsDialog(
    LauncherSettings* settings, QWidget* pParent /*= nullptr*/ )
    : QDialog( pParent ), m_pSettings( settings )
{
    this->setupUi( this );

#ifdef _WIN32
    this->launchScriptInput->setEnabled( false );
    this->scriptSearchBtn->setEnabled( false );
#endif

    this->LoadSettings();
    this->ConnectActions();
}

void CLauncherSettingsDialog::ConnectActions()
{
    this->connect( this->scriptSearchBtn, &QPushButton::pressed, this,
                   &CLauncherSettingsDialog::OnScriptBrowseBtn );
    this->connect( this->buttonBox, &QDialogButtonBox::accepted, this,
                   &CLauncherSettingsDialog::OnAccepted );
}

void CLauncherSettingsDialog::OnAccepted()
{
    this->SaveSettings();
}

void CLauncherSettingsDialog::OnScriptBrowseBtn()
{
    this->launchScriptInput->setText( QFileDialog::getOpenFileName(
        this, tr( "Choose the launch script" ), QDir::currentPath() ) );
}

void CLauncherSettingsDialog::LoadSettings()
{
    this->m_pSettings->LoadFromFile();

    this->enableConsoleInput->setChecked(
        this->m_pSettings->IsConsoleEnabled() );
    this->unpackedFilesInput->setChecked(
        this->m_pSettings->ShouldUseUnpackedFiles() );
    this->langInput->setText( this->m_pSettings->GetLanguageFile() );
    this->customArgsInput->setText(
        this->m_pSettings->GetAdditionalArgs().join( ' ' ) );
    this->launchScriptInput->setText( this->m_pSettings->GetLaunchScript() );
}

void CLauncherSettingsDialog::SaveSettings() const
{
    this->m_pSettings->SetConsoleEnabled(
        this->enableConsoleInput->isChecked() );
    this->m_pSettings->SetUseUnpackedFiles(
        this->unpackedFilesInput->isChecked() );
    this->m_pSettings->SetLanguageFile( this->langInput->text() );
    this->m_pSettings->SetAdditionalArgs( this->customArgsInput->text().split(
        QRegExp( "\\s+" ), QString::SkipEmptyParts ) );
    this->m_pSettings->SetLaunchScript( this->launchScriptInput->text() );

    this->m_pSettings->SaveToFile();
}
