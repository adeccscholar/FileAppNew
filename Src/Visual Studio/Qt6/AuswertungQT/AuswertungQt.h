#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AuswertungQt.h"

#include <QLabel>
#include <QMenu>
#include <map>
#include <FileAppProcess.h>


class AuswertungQt : public QMainWindow
{
    Q_OBJECT

public:
    AuswertungQt(QWidget *parent = nullptr);
    ~AuswertungQt();

private:
    Ui::AuswertungQtClass ui;
    QLabel* statusLabel;

    TProcess proc;
    std::map<EShowVariante, QMenu*> mpCtxMenu;
   



private slots:
   void ShowContextMenu(QPoint const&);
};
