#include "AuswertungQt.h"
#include <QMessagebox>
#include <utility>

AuswertungQt::AuswertungQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.statusBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("sbMain");
    statusLabel->setText("sbMain");
    ui.statusBar->addPermanentWidget(statusLabel);

    ui.lvOutput->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.btnCount, &QPushButton::clicked, this, [this]() { this->proc.CountAction(); });
    connect(ui.btnShow,  &QPushButton::clicked, this, [this]() { this->proc.ShowAction(); });
    connect(ui.btnParse, &QPushButton::clicked, this, [this]() { this->proc.ParseAction(); });

    connect(ui.lvOutput, SIGNAL(customContextMenuRequested(QPoint const&)), this, SLOT(ShowContextMenu(QPoint const&)));

    try {
       proc.Init({ this, false });
    }
    catch (std::exception& ex) {
       QMessageBox msg;
       msg.setText(ex.what());
       msg.exec();
    }

}

AuswertungQt::~AuswertungQt(){ 
   // delete statusLabel;
}

void AuswertungQt::ShowContextMenu(QPoint const& pos) {
   switch (auto mode = proc.GetShowMode(); mode) {
      case EShowVariante::Projects:
         {
         QMenu contextMenu("context menu for projects", this);
         QAction actProject("open project file", this);
         QAction actSource("open cpp file", this);
         QAction actHeader("open h file", this);
         QAction actRes("open res file", this);
         connect(&actProject, &QAction::triggered, this, [this]() { this->proc.OpenProjectFile(); });
         connect(&actSource,  &QAction::triggered, this, [this]() { this->proc.OpenSourceFile(); });
         connect(&actHeader,  &QAction::triggered, this, [this]() { this->proc.OpenHeaderFile(); });
         connect(&actRes,     &QAction::triggered, this, [this]() { this->proc.OpenResFile(); });
         contextMenu.addAction(&actProject);
         contextMenu.addAction(&actSource);
         contextMenu.addAction(&actHeader);
         contextMenu.addAction(&actRes);
         contextMenu.exec(ui.lvOutput->mapToGlobal(pos));
         } break;
      case EShowVariante::Files: 
      {
         QMenu contextMenu("context menu for files", this);
         QAction actFile("open file", this);
         connect(&actFile, &QAction::triggered, this, [this]() { this->proc.OpenViewFile(); });
         contextMenu.addAction(&actFile);
         contextMenu.exec(ui.lvOutput->mapToGlobal(pos));
      } break;
      default:
      QMessageBox box;
      box.setText("there isn't a menu to show");
      box.exec();
      }
}

