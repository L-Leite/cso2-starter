#pragma once

#include <QDialog>
#include "ui_aboutdialog.h"

class CAboutDialog : public QDialog, public Ui::AboutDialog
{
    Q_OBJECT

public:
    CAboutDialog( QWidget* pParent = nullptr );
};
