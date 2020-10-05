#include "aboutdialog.hpp"

#include "starter_version.hpp"

CAboutDialog::CAboutDialog( QWidget* pParent /*= nullptr*/ )
    : QDialog( pParent )
{
    this->setupUi( this );

    this->verLabel->setText( STARTER_VERSION );
    this->commitLabel->setText( GIT_COMMIT_HASH "-" GIT_BRANCH );
}
