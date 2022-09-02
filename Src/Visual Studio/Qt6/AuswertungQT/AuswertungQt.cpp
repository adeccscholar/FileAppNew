#include "AuswertungQt.h"
#include <QMessagebox>
#include <vector>
#include <utility>

struct my_control_for_find { };

template <typename ty>
typename ty::mapped_type& my_find(ty& container, typename ty::key_type const& value) {
   auto it = container.find(value);
   if (it == container.end()) throw my_control_for_find();
   else return it->second;
   }

void my_message(QWidget* parent, QString const& text) {
   QMessageBox::information(parent, "FileApp - Information", text);
   }

AuswertungQt::AuswertungQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.statusBar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("sbMain");
    statusLabel->setText("sbMain");
    ui.statusBar->addPermanentWidget(statusLabel);

    connect(ui.btnCount, &QPushButton::clicked, this, [this]() { this->proc.CountAction(); });
    connect(ui.btnShow,  &QPushButton::clicked, this, [this]() { this->proc.ShowAction(); });
    connect(ui.btnParse, &QPushButton::clicked, this, [this]() { this->proc.ParseAction(); });

    ui.lvOutput->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.lvOutput, &QWidget::customContextMenuRequested, this, &AuswertungQt::ShowContextMenu);

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
                                                  my_message(this, QString::fromStdString(os.str()));
                                                 } },
          { "Lines in selected Files", [this]() { std::ostringstream os;
                                                  this->proc.CountFileRowsForProjects(os, true);
                                                  my_message(this, QString::fromStdString(os.str()));
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
   try {  
      if (auto menu = my_find(mpCtxMenu, proc.GetShowMode()); menu )
      menu->exec(ui.lvOutput->mapToGlobal(pos));
      }
   catch(my_control_for_find& ex) { my_message(this, "there isn't a menu to show");  }
   }

