#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AuswertungQt.h"

#include <QLabel>
#include <QMenu>
#include <map>
#include "FileAppProcessQT.h"


class AuswertungQt : public QMainWindow
{
    Q_OBJECT

public:
    AuswertungQt(QWidget *parent = nullptr);
    ~AuswertungQt();

private:
    Ui::AuswertungQtClass ui;
    QLabel* statusLabel;

    TProcessQT proc;
    std::map<EShowVariante, QMenu*> mpCtxMenu;


};
