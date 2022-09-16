#include "AuswertungQt.h"
#include <QMessagebox>
#include <vector>
#include <utility>

#include "MyAlgorithm.h"



AuswertungQt::AuswertungQt(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    ui.statusBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("sbMain");
    statusLabel->setText("sbMain");
    ui.statusBar->addPermanentWidget(statusLabel);

    //connect(ui.btnSelect,           &QPushButton::clicked, this, [this]() { this->proc.SelectWithDirDlg(); });
    connect(ui.btnSelect, &QPushButton::clicked, this, [this]() { this->proc.Test2(); });

    connect(ui.btnCount,            &QPushButton::clicked, this, [this]() { this->proc.CountAction(); });
    connect(ui.btnShow,             &QPushButton::clicked, this, [this]() { this->proc.ShowAction(); });
    connect(ui.btnParse,            &QPushButton::clicked, this, [this]() { this->proc.ParseAction(); });
    connect(ui.btnAddExtention,     &QPushButton::clicked, this, [this]() { this->proc.AddExtention(); });
    connect(ui.btnChgExtention,     &QPushButton::clicked, this, [this]() { this->proc.ChangeSelectedExtentions(); });
    connect(ui.btnDelExtention,     &QPushButton::clicked, this, [this]() { this->proc.DeleteExtentions(true); });
    connect(ui.btnDelAllExtentions, &QPushButton::clicked, this, [this]() { this->proc.DeleteExtentions(false); });
    connect(ui.btnCloseApp,         &QPushButton::clicked, this, [this]() { this->close(); });

    connect(ui.lbValues, &QListWidget::itemSelectionChanged, this, [this]() { this->proc.SelectedExtentionsChanged(); });

    ui.lvOutput->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect(ui.lvOutput, &QWidget::customContextMenuRequested, this, &AuswertungQt::ShowContextMenu);
    connect(ui.lvOutput, &QWidget::customContextMenuRequested, this, [this](QPoint const& pos) {
       try {
          if (auto menu = my_find(this->mpCtxMenu, this->proc.GetShowMode()); menu)
             menu->exec(this->ui.lvOutput->mapToGlobal(pos));
       }
       catch (my_control_for_find& ex) {
          this->proc.Form().Message(EMyMessageType::error, "FileApp - Information", "there isn't a menu to show");
       }
       });


    using MenuActions = std::vector<std::pair<QString, std::function<void()>>>;
    auto CreateCtxMenu = [this](QMenu* parent, MenuActions const& actions) {
       for (auto& [text, func] : actions) {
          if (func != nullptr) {
             auto act = new QAction(text, parent);
             connect(act, &QAction::triggered, this, func);
             parent->addAction(act);
             }
          else {
             parent->addSeparator();
             }
          }
       };

    if(auto [menu, success] = mpCtxMenu.insert({ EShowVariante::Projects, new QMenu("context menu for projects", this) }); success) {
       MenuActions actions = {
          { "open project file",       [this]() { this->proc.OpenProjectFile(); } },
          { "-----------------",       nullptr },
          { "open cpp file",           [this]() { this->proc.OpenSourceFile(); } },
          { "open h file",             [this]() { this->proc.OpenHeaderFile(); } },
          { "open res file",           [this]() { this->proc.OpenResFile(); } },
          { "-----------------",       nullptr },
          { "Lines in all Files",      [this]() { std::ostringstream os;
                                                  this->proc.CountFileRowsForProjects(os, false);
                                                  this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                                 } },
          { "Lines in selected Files", [this]() { std::ostringstream os;
                                                  this->proc.CountFileRowsForProjects(os, true);
                                                  this->proc.Form().Message(EMyMessageType::information, "FileApp - Information", os.str());
                                                 } }
       };
       CreateCtxMenu(menu->second, actions);
       }


   if (auto [menu, success] = mpCtxMenu.insert({ EShowVariante::Files, new QMenu("context menu for files", this) }); success) {
      MenuActions actions = {
          {  "open file", [this]() { this->proc.OpenViewFile(); } }
          };
      CreateCtxMenu(menu->second, actions);
      }

   mpCtxMenu.insert({ EShowVariante::Count, nullptr } );

   try {
      proc.Init( { this, false } );
      }
   catch (std::exception& ex) {
       proc.Form().Message(EMyMessageType::error, "FileApp - Information", ex.what());
       }

   }

AuswertungQt::~AuswertungQt(){ 
   }


