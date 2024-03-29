﻿/**
 \file
 \brief   file with the implementation of independent parts of the process logic for the FileApp program
 \details This file contain the independent implementation of the processes in this application.
          The core is the class TProcess.
 \details By the use of the header-only library adecc Scholar the independence of the respective framework
          is achieved.
 <hr>
 \author Volker Hillmann (adecc Systemhaus GmbH)
 \date 22.07.2022 Create repository with this file
 \version 0.1
 \since Version 0.1
*/
#pragma hdrstop
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>

#include "FileAppProcess.h"
#include "MyFileDlg.h"

#include <adecc_Scholar/MyForm.h>
#include <adecc_Scholar/MyStream.h>
#include <adecc_Scholar/MyTools.h>
#include <adecc_Scholar/MyType_Traits.h>
#include <adecc_Scholar/MyTupleUtils.h>
#include <adecc_Scholar/MyLogger.h>
#include "MyFileException.h"
#include "MyFileDlg.h"
#include "FileUtil.h"

#include "pugixml.hpp"


#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
#include <exception>
#include <fstream>
#include <execution>
#include <regex>



// --------------------------------------------------------------------------
// Prozesslogik

struct TMyNum : public std::numpunct<char> {
	char_type   do_decimal_point () const { return ','; }
	char_type   do_thousands_sep () const { return '.'; }
	string_type do_truename ()      const { return "ja"; }
	string_type do_falsename ()     const { return "nein"; }
	std::string do_grouping ()      const { return "\3";	}
   };

TMyNum newNumPunct;
std::locale TProcess::myLoc;

TStreamWrapper<Latin> old_cout(std::cout);
TStreamWrapper<Latin> old_cerr(std::cerr);
TStreamWrapper<Latin> old_clog(std::clog);

/// extensions for c++ builder project files
std::set<std::string> TProcess::project_extensions = { ".cbproj" };
std::set<std::string> TProcess::header_files = { ".h", ".hxx", ".hpp" };
std::set<std::string> TProcess::form_files = { ".dfm", ".fmx" };



/// vector with captions and params for the project list
std::vector<tplList<Latin>> TProcess::Project_Columns {
    		  tplList<Latin> { "project",      360, EMyAlignmentType::left },
           tplList<Latin> { "path",         650, EMyAlignmentType::left },
           tplList<Latin> { "type",         150, EMyAlignmentType::left },
			  tplList<Latin> { "order",        110, EMyAlignmentType::right },
    		  tplList<Latin> { "cpp- file",    350, EMyAlignmentType::left },
    		  tplList<Latin> { "rows",         110, EMyAlignmentType::right },
			  tplList<Latin> { "h- file",      280, EMyAlignmentType::left },
			  tplList<Latin> { "rows",         110, EMyAlignmentType::right },
			  tplList<Latin> { "form file",    350, EMyAlignmentType::left },
			  tplList<Latin> { "form name",    340, EMyAlignmentType::left },
			  tplList<Latin> { "rows",         110, EMyAlignmentType::right },
			  tplList<Latin> { "formType",     150, EMyAlignmentType::left },
			  tplList<Latin> { "designclass",  250, EMyAlignmentType::left } 
           };

/// vector with captions and params for counting files
std::vector<tplList<Latin>> TProcess::Count_Columns {
    		  tplList<Latin> { "files",        450, EMyAlignmentType::right },
           tplList<Latin> { "directories",  450, EMyAlignmentType::right },
           tplList<Latin> { "size",         600, EMyAlignmentType::right }
           };

/// vector with captions and params for the file list
std::vector<tplList<Latin>> TProcess::File_Columns {
    		  tplList<Latin> { "file",        450, EMyAlignmentType::left },
           tplList<Latin> { "directory",   900, EMyAlignmentType::left },
           tplList<Latin> { "time",        265, EMyAlignmentType::left },
           tplList<Latin> { "size",        150, EMyAlignmentType::right } 
           };


constexpr int iMyData_Project  =  0; ///< constant for position of name of project in tplData
constexpr int iMyData_Path     =  1; ///< constant for position of path to project in tplData
constexpr int iMyData_Type     =  2; ///< constant for type of node in tplData
constexpr int iMyData_Order    =  3; ///< constant for build order in tplData
constexpr int iMyData_CppFile  =  4; ///< constant for source file with relative path in tplData
constexpr int iMyData_CppRows  =  5; ///< constant for rows in source file in tplData
constexpr int iMyData_H_File   =  6; ///< constant for name with relative path of dependent header file
constexpr int iMyData_H_Rows   =  7; ///< constant for rows in header file in tplData
constexpr int iMyData_FrmFile  =  8; ///< constant for name with relative path of dependent form file in tplData
constexpr int iMyData_FrmName  =  9; ///< constant for name of the formular in dependent form file in tplData
constexpr int iMyData_FrmRows  = 10; ///< constant for rows in form file in tplData
constexpr int iMyData_FrmType  = 11; ///< constant for type of form in dependent form file in tplData
constexpr int iMyData_FrmClass = 12; ///< constant for used design class in tplData




template <typename string_type, typename container>
size_t parse(string_type const& source, std::string const& del, container& list) {
   static_assert(is_vector<container>::value || is_set<container>::value, "unexpected value for container in function parse");
   size_t pos = 0u, end;
   do {
       end = source.find(del, pos);
       string_type part = source.substr(pos, end - pos);
       if(auto start_at = part.find_first_not_of(" \t"); start_at != string_type::npos) {
          auto end_at   = part.find_last_not_of(" \t");
          string_type trimmedpart = part.substr(start_at, end_at - start_at + 1);
          if(trimmedpart.length() > 0) {
             if constexpr (is_vector<container>::value) {
                list.emplace_back(trimmedpart);
                }
             else if constexpr (is_set<container>::value) {
                list.insert(trimmedpart);
                }
             else {
                static_assert_no_match();
                }
             }
          }
      pos = end + del.length();
      } while (end != string_type::npos);
   return list.size();
   }





 /**
   \brief Intitialize the main window of the application
   \param form [IN] rvalue to form, which is used as main window
   \throw std::runtime_error
*/
 void TProcess::Init(TMyForm&& form) {
   swap(frm, form);
   std::ios_base::sync_with_stdio(false);
   myLoc = std::locale(std::locale("de_DE"), &newNumPunct);
   std::locale::global(myLoc);

   frm.GetAsStream<Latin, EMyFrameworkType::listview>(old_cout, "lvOutput", Project_Columns);
   frm.GetAsStream<Latin, EMyFrameworkType::memo>(old_cerr, "memError");
   frm.GetAsStream<Latin, EMyFrameworkType::statusbar>(old_clog, "sbMain");
   showMode = EShowVariante::Projects;

   for(auto stream : { &std::cout, &std::cerr, &std::clog } ) {
      stream->imbue(myLoc);
      stream->setf(std::ios::fixed);
      }

   frm.SetCaption("analyzing projects (Framework: " + frm.GetFramework() + ") ...");

   frm.Set<EMyFrameworkType::label>("lblDirectory", "directory:");
   frm.Set<EMyFrameworkType::edit>("edtDirectory", "d:\\projekte\\vorlesung");
   frm.Set<EMyFrameworkType::checkbox>("chbSubDirs", "Unterverzeichnisse einbeziehen");
   frm.Set<EMyFrameworkType::checkbox>("chbSubDirs", true);

   frm.Set<EMyFrameworkType::button>("btnCount", "count");
   frm.Set<EMyFrameworkType::button>("btnShow",  "show");     // !!!
   frm.Set<EMyFrameworkType::button>("btnParse", "parse");

   frm.Set<EMyFrameworkType::edit>("edtExtentions", "");

   frm.Set<EMyFrameworkType::button>("btnSelect", "Select");
   frm.Set<EMyFrameworkType::button>("btnAddExtention", "Add");
   frm.Set<EMyFrameworkType::button>("btnChgExtention", "Change");
   frm.Set<EMyFrameworkType::button>("btnDelExtention", "Delete");
   frm.Set<EMyFrameworkType::button>("btnDelAllExtentions", "Delete All");
   frm.Set<EMyFrameworkType::button>("btnCloseApp", "Beenden");

   frm.Set<EMyFrameworkType::button>("btnTest", "Testroutine");


   std::ostream mys(frm.GetAsStreamBuff<Latin, EMyFrameworkType::listbox>("lbValues"), true);
   std::vector<std::string> test = { ".cpp", ".h", ".dfm", ".fmx", ".cbproj", ".c", ".hpp" };
   std::copy(test.begin(), test.end(), std::ostream_iterator<std::string>(mys, "\n"));

   std::clog << "program ready for action" << std::endl;

   }

void TProcess::Test4() { 
   TMyFileDlg::Message(EMyMessageType::warning, "Aufgepaßt", "Testfehler\r\n2.Zeile\r\nes geht noch mehr!");
   //std::string strTest = "Hallo Welt";
   int iVal = 42;
   EMyRetResults ret;
   if(std::tie(ret, iVal) = TMyFileDlg::Input(iVal);ret == EMyRetResults::ok) {
      std::ostringstream os;
      os << "Wert = " << iVal;
      TMyFileDlg::Message(EMyMessageType::information, "Eingabe bestätigt, neuer Wert", os.str()); 
      }
   else {
      TMyFileDlg::Message(EMyMessageType::warning, "Eingabe abgebrochen", "Der Wert wurde nicht geändert."); 
      }
   }

void TProcess::Test3() {
   TMyToggle toggle("Guard for boActive", boActive);
 
   const auto copyOptions = fs::copy_options::update_existing;
   std::vector<fs::path> files;
   std::set<std::string> extensions;
   my_formlist<EMyFrameworkType::listbox, std::string> mylist(&frm, "lbValues");
   std::copy(mylist.begin(), mylist.end(), std::inserter(extensions, extensions.end()));

   fs::path base("D:\\Projekte\\GitHub");
   auto target = fs::path{ "D:\\Test\\Sicherung" };
   EMyRetResults ret;
   std::tie(ret, target) = TMyFileDlg::Input(target, "Kopieren eines Verzeichnisses", "Geben Sie das Ziel an:");
   if (ret != EMyRetResults::ok) {
      TMyFileDlg::Message(EMyMessageType::warning, "Nutzerabbruch", "Die Auswahl wurde abgebrochen.\nBearbeitung beendet.");
      return;
      }
   auto func_start = std::chrono::high_resolution_clock::now();
   Find(files, base, extensions, true);
   #if !defined __BORLANDC__
      for_each(std::execution::par, files.cbegin(), files.cend(), [base, target, copyOptions](auto const& p) {
   #else
      for_each(files.cbegin(), files.cend(), [base, target, copyOptions](auto const& p) { 
   #endif
      try {
              fs::path file = fs::relative(p, base);
              file = target / file;
              if (!fs::exists(file.parent_path())) fs::create_directories(file.parent_path());
              fs::copy_file(p, file, copyOptions);
              }
           catch (fs::filesystem_error const& ex) {
              std::cerr << p.string() << " can't copied, " << ex.what() << std::endl;
              }
           catch (std::bad_alloc const& ex) {
              std::cerr << p.string() << " can't copied, " << ex.what() << std::endl;
              }
           });
   auto func_ende = std::chrono::high_resolution_clock::now();
   auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
   std::clog << "copy files finished after" << time.count() / 1000. << " sec" << std::endl;
}

void TProcess::Test2() {
   try {
      TMyFileDlg::OpenFileAction("D:\\unknown\\wrong\\file.txt");
      }
   catch(my_file_information const& ex) {
      throw my_file_runtime_error(ex, MY_POSITION());
      }
}

void TProcess::Test() {
   std::vector<std::pair<std::string, std::string>> test_cases = {
      { "d:\\test\\Berlin\\Spandau\\Wilhelmstadt\\Bocksfeldplatz.csv"s, "korrekt: absoluter Pfad mit Datei (ohne Leerzeichen)"s },
      { "d:\\test\\Berlin\\Spandau\\Wilhelmstadt\\Charlottenburger Chaussee.csv"s, "korrekt: absoluter Pfad mit Datei ( mit Leerzeichen)"s },
      { "d:\\test\\Berlin\\Spandau\\Wilhelmstadt"s, "korrekt: absoluter Pfad zu Verzeichnis"s },
      { "d:\\test\\Berlin\\Spandau\\Wilhelmstadt\\Agathenweg.csv"s, "Pfad existiert, aber Datei nicht"s },
      { "d:\\test\\Berlin\\Kladow\\Wilhelmstadt"s, "Fehler, absoluter Pfad existiert nicht"s },
      { "d:\\test\\Berlin\\Kladow\\Wilhelmstadt\\Bocksfeldplatz.csv"s, "Fehler: absoluter Pfad, Pfad exitiert, aber Datei nicht"s },
      { "d:\\test\\Berlin\\Spandau\\Tegel"s, "Fehler, absultuer Pfad, Verzeichnis exisiert nicht"s },
      { "d:\\test\\Berlin"s, "korrekt, absolute Angabe, Teilpfad"s },
      { "d:\\test"s, "korrekt, absolute Angabe, noch kürzerer Teilpfad"s },
      { "d:\\test\\testfile.txt"s, "korrekt, Datei im ersten Verzeichnis, Datei existiert"s },
      { "d:\\test\\testfile.csv"s, "Fehler, Datei im ersten Verzeichnis, Datei existiert nicht"s },
      { "d:\\"s, "korrekt, nur Root mit Verzeichniszeichen"s },
      { "d:\\testfile.txt"s, "korrekt, Datei im Rootverzeichnis"s },
      { "d:\\testfile.csv"s, "Fehler, Datei im Rootverzeichnis extiert nicht"s },
      { "test\\Berlin\\Spandau\\Wilhelmstadt"s, "Fehler, Pfad ohne Root und ohne Dot"s },
      { "Spandau\\Wilhelmstadt"s, "Fehler, Teilpfad ohne Root, ohne Dot"s },
      { "Bocksfeldplatz.csv"s, "Fehler, Angabe einer Datei ohne Pfad"s },
      { "\\test\\Berlin\\Spandau\\Wilhelmstadt\\Bocksfeldplatz.csv"s, "korrekt wenn Root D:, Datei, Angabe relativer Pfad"s },
      { "\\test\\Berlin\\Spandau"s, "korrekt wenn Root D:, Verzeichnis, Angabe relativer Pfad"s },
      { "test\\Berlin\\Spandau\\Wilhelmstadt\\Bocksfeldplatz.csv"s, "Fehler, Pfad wäre richtig, aber kein relativer Pfad"s },
      { "test\\Berlin\\Spandau\\Tegel\\Bocksfeldplatz.csv"s, "Fehler, Pfad wäre falsch und ist kein relativer Pfad"s },
      { "test\\Berlin\\Spandau"s, "Fehler, Pfad wäre richtig, ist aber kein relativer Pfad"s },
      { "\\"s, "Nur die Root angegeben, sollte immer richtig sein, als Verzeichnis"s },
      { "\\Test"s, "Korrekt, wenn Root D:, Verzeichnis Test als relativer Pfad"s },
      { "\\Test22"s, "Fehler, wenn Verzeichnis Test als relativer Pfad, existiert aber nicht"s },
      { "\\testfile.txt"s, "Korrekt, wenn Root D:, Datei im Rootverzeichnis extiert"s },
      { "\\testfile.csv"s, "Fehler, Datei im Rootverzeichnis extiert nicht"s }
      };
     
   std::cerr << fs::current_path().string() << std::endl
             << fs::temp_directory_path().string() << std::endl
             << std::endl;

   for(auto const& [test_case, description] : test_cases) {
      fs::path thePath(test_case);

      std::cerr << thePath.string() << std::endl
                << description << std::endl
                << fs::weakly_canonical(thePath).string() << std::endl
                << "exists:             " << (fs::exists(thePath) ? "ja" : "nein") << std::endl
                << "absolut:            " << (thePath.is_absolute() ? "ja" : "nein") << std::endl
                << "relativ:            " << (thePath.is_relative() ? "ja" : "nein") << std::endl
                << "is directory:       " << (fs::is_directory(thePath) ? "ja" : "nein") << std::endl
                << "is file:            " << (fs::is_regular_file(thePath) ? "ja" : "nein") << std::endl
                << "hat Verzeichnis:    " << (thePath.has_parent_path() ? "ja" : "nein") << std::endl
                << "hat Filename:       " << (thePath.has_filename() ? "ja" : "nein") << std::endl
                << "hat Erweiterung     " << (thePath.has_extension() ? "ja" : "nein") << std::endl
                << "has root_name:      " << (thePath.has_root_name() ? "ja" : "nein") << std::endl
                << "has root_dir:       " << (thePath.has_root_directory() ? "ja" : "nein") << std::endl
                << "has root_path:      " << (thePath.has_root_path() ? "ja" : "nein") << std::endl
                << "root_name:          " << thePath.root_name().string() << std::endl
                << "root_dir:           " << thePath.root_directory().string() << std::endl
                << "root_path:          " << thePath.root_directory().string() << std::endl
                << "parent path:        " << thePath.parent_path().string() << std::endl
                << "file name:          " << thePath.filename().string() << std::endl
                << "parent path exists: " << (fs::exists(thePath.parent_path()) ? "ja" : "nein") << std::endl
                << std::endl;
   }

}

void TProcess::SelectWithDirDlg() {
  // auto path = Form().Get<EMyFrameworkType::edit, std::string>("edtDirectory");
   
   switch(auto [ret, strFile] = TMyFileDlg::SelectWithFileDirDlg(Form().Get<EMyFrameworkType::edit, std::string>("edtDirectory"), true); ret) {
      case EMyRetResults::ok: 
         Form().Set<EMyFrameworkType::edit>("edtDirectory", strFile);
         break;
      case EMyRetResults::error:
         TMyFileDlg::Message(EMyMessageType::error, "FileApp - Programm", strFile);
         //Form().Message(EMyMessageType::error, "FileApp - Programm", strFile);
         break;
      default:
         TMyFileDlg::Message(EMyMessageType::information, "FileApp - Programm", "Auswahl abgebrochen.");
         //Form().Message(EMyMessageType::information, "FileApp - Programm", "Auswahl abgebrochen.");
      }
   }


 void TProcess::AddExtention() {
    static const std::string strCaption = "Eingabefehler in TProcess::AddExtention";
    try {
       static std::regex parser("\\.[A-Za-z0-9]+(,\\.[A-Za-z0-9]+)*");
       auto strExtention = frm.Get <EMyFrameworkType::edit, std::string>("edtExtentions");
       if(strExtention) {
          if(std::regex_match(*strExtention, parser)) {
             std::set<std::string> setExtentions;
             my_formlist<EMyFrameworkType::listbox, std::string> extentions(&frm, "lbValues");
             std::copy(extentions.begin(), extentions.end(), std::inserter(setExtentions, setExtentions.end()));
             std::vector<std::string> vecInput;
       
             parse(*strExtention, ",", vecInput);
             for(auto const& item : vecInput) {
                if(setExtentions.find(item) == setExtentions.end()) {
                   frm.AddListBox("lbValues", item);
                   }
                else {
                   std::ostringstream os;
                   os << "extention \"" << item << "\" can't inserted, the extention is in the list already.";
                   TMyFileDlg::Message(EMyMessageType::error, strCaption, os.str());
                   //frm.Message(EMyMessageType::error, strCaption, os.str());
                   }
                }
             }
          else {
             std::ostringstream os;
             os << "the input \"" << *strExtention << "\" in field \"edtExtentions\" isn't valid for extentions.";
             TMyFileDlg::Message(EMyMessageType::error, strCaption, os.str());
             //frm.Message(EMyMessageType::error, strCaption, os.str());
             }
          }
       else {
          TMyFileDlg::Message(EMyMessageType::error, strCaption, "input field \"edtExtentions\" is empty.");
          //frm.Message(EMyMessageType::error, strCaption, "input field \"edtExtentions\" is empty.");
          }
       }
    catch (std::exception& ex) {
       std::cerr << "error in TProcess::AddExtention: " << ex.what() << std::endl;
       }
    }

void TProcess::ChangeSelectedExtentions(void) {
   static const std::string strCaption = "Eingabefehler in TProcess::ChangeSelectedExtentions";
   try {
      auto rows = frm.GetSelectedRows<EMyFrameworkType::listbox>("lbValues");
      switch(rows.size()) {
         case 0:
            TMyFileDlg::Message(EMyMessageType::error, strCaption, "no rows selected in the box to change");
            //frm.Message(EMyMessageType::error, strCaption, "no rows selected in the box to change");
            break;
         case 1: {
            auto strExtention = frm.Get<EMyFrameworkType::edit, std::string>("edtExtentions");
            if (strExtention) {
               static std::regex parser("\\.[A-Za-z0-9]+");
               if (std::regex_match(*strExtention, parser)) {
                  std::vector<std::string> vecInput;
                  parse(*strExtention, ",", vecInput);
                  switch (vecInput.size()) {
                     case 0:
                        TMyFileDlg::Message(EMyMessageType::error, strCaption, "the input field \"edtExtentions\" is empty or invalid");
                        //frm.Message(EMyMessageType::error, strCaption, "the input field \"edtExtentions\" is empty or invalid");
                        break;
                     case 1: {
                        my_formlist<EMyFrameworkType::listbox, std::string> extentions(&frm, "lbValues");
                        auto it = std::find_if(extentions.begin(), extentions.end(), [&vecInput](auto val) { return val == vecInput[0]; });
                        if (it == extentions.end()) {
                           frm.SetValue<EMyFrameworkType::listbox>("lbValues", rows[0], 0, vecInput[0]);
                           }
                        else {
                           std::ostringstream os;
                           os << "entered extension \"" << vecInput[0] << "\" is already included.";
                           TMyFileDlg::Message(EMyMessageType::error, strCaption, os.str());
                           //frm.Message(EMyMessageType::error, strCaption, os.str());
                           }
                        } break;
                     default: {
                        std::ostringstream os;
                        os << *strExtention << " has more as one extentions in input \"edtExtentions\" (" << vecInput.size() << ").";
                        TMyFileDlg::Message(EMyMessageType::error, strCaption, os.str());
                        //frm.Message(EMyMessageType::error, strCaption, os.str());
                        }
                     }
                  }
               else {
                  std::ostringstream os;
                  os << "the input \"" << *strExtention << "\" in \"edtExtentions\" isn't a valid extention.";
                  TMyFileDlg::Message(EMyMessageType::error, strCaption, os.str());
                  //frm.Message(EMyMessageType::error, strCaption, os.str());
                  }
               }
            else {
               TMyFileDlg::Message(EMyMessageType::error, strCaption, "input field \"edtExtentions\" is empty.");
               //frm.Message(EMyMessageType::error, strCaption, "input field \"edtExtentions\" is empty.");
               }
            } break;
         default:
            TMyFileDlg::Message(EMyMessageType::error, strCaption, "more as one row selected, operation isn't available.");
            //frm.Message(EMyMessageType::error, strCaption, "more as one row selected, operation isn't available.");
         }
      }
   catch(std::exception &ex) {
      std::cerr << "error in method TProcess::ChangeSelectedExtentions: " << ex.what() << std::endl;
      }
   }

void TProcess::DeleteExtentions(bool boSelectedOnly) {
   static const std::string strCaption = "Eingabefehler in TProcess::DeleteExtentions";
   try {
      // auto rows = frm.GetSelectedRows<EMyFrameworkType::listbox>("lbValues");

      using Ret_Type = decltype(frm.GetSelectedRows<EMyFrameworkType::listbox>("lbValues"));
      Ret_Type rows;
      if(boSelectedOnly) rows = frm.GetSelectedRows<EMyFrameworkType::listbox>("lbValues");
      else               rows = frm.GetAllRows<EMyFrameworkType::listbox>("lbValues");
      if(rows.size() >= 1) {
         std::sort(rows.begin(), rows.end(), [](auto lhs, auto rhs) { return lhs > rhs; });
         for(auto row : rows) frm.Delete_Value_in_list<EMyFrameworkType::listbox>("lbValues", row);
         }
      else {
         TMyFileDlg::Message(EMyMessageType::error, strCaption, "no selected rows in listbox with extentions");
         //frm.Message(EMyMessageType::error, strCaption, "no selected rows in listbox with extentions");
         }
      }
   catch(std::exception &ex) {
      std::cerr << "error in TProcess::DeleteSelectedExtentions: " << ex.what() << std::endl;
      }
   }


/*

      if(GetShowMode() == EShowVariante::Files || GetShowMode() == EShowVariante::Projects) {
         auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
         if(strPath) {
            auto selected = frm.GetSelectedRows<EMyFrameworkType::listview>("lvOutput");
            if(selected.size() ==1) {
               auto relpath = frm.GetValue<EMyFrameworkType::listview, std::string>("lvOutput", selected[0], dir);
               auto relfile = frm.GetValue<EMyFrameworkType::listview, std::string>("lvOutput", selected[0], file);
               if(relpath && relfile) {
                  auto file_to_open = fs::weakly_canonical(fs::path(*strPath) / fs::path(*relpath) / fs::path(*relfile));
                  TMyFileDlg::OpenFileAction(Form(), file_to_open.string());
                  }
               else {
                  std::cerr << "Can't open file, missing information in selected row" << std::endl;
                  }
               }
            else {
               std::cerr << "open files only with single selected row" << std::endl;
               }
            }
         else {
            std::cerr << "open files only if directory exist" << std::endl;
            }

*/


void TProcess::OpenFileInDirectory(void) {
   TMyToggle toggle("Guard for boActive", boActive);
   try {
      auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
      if (!strPath) {
         TMyLogger log(__func__, __FILE__, __LINE__);
         log.stream() << "directory to show is empty, set a directory before call this function";
         log.except();
         }
      else {
         fs::path fsPath;
         if(auto mode = GetShowMode(); mode == EShowVariante::Files || mode == EShowVariante::Projects) {
            auto selected = frm.GetSelectedRows<EMyFrameworkType::listview>("lvOutput");
            if(selected.size() !=1) throw std::runtime_error("open files only with single selected row");
            auto relpath = frm.GetValue<EMyFrameworkType::listview, std::string>("lvOutput", selected[0], 1);
            if(!relpath) throw std::runtime_error("Can't select directory, missing information in selected row");
            fsPath = fs::weakly_canonical(fs::path(*strPath) / fs::path(*relpath));
            }
         else {
            fsPath = *strPath;
            }

         switch(auto [ret, strFile] = TMyFileDlg::SelectWithFileDirDlg(std::optional<std::string> { fsPath.string() }, false); ret) {
            case EMyRetResults::ok: 
               TMyFileDlg::OpenFileAction(strFile);
               break;
            case EMyRetResults::error:
               TMyFileDlg::Message(EMyMessageType::error, "FileApp - Programm", strFile);
               break;
            default:
               TMyFileDlg::Message(EMyMessageType::information, "FileApp - Programm", "Auswahl abgebrochen.");
            }
         }
      }
   catch(std::exception& ex) {
      TMyFileDlg::Message(EMyMessageType::error, "FileApp - Programm", ex.what());
      }
   }

void TProcess::SelectedExtentionsChanged(void) {
   try {
      auto rows = frm.GetSelectedRows<EMyFrameworkType::listbox>("lbValues");
      std::ostringstream os;
      int i = 0;
      std::for_each(rows.begin(), rows.end(), [&os, this, &i](auto const& row) {
           auto text = this->frm.GetValue<EMyFrameworkType::listbox, std::string>("lbValues", row, 0);
           os << (i++ > 0 ? "," : "") << text.value_or("");
           });
      frm.Set<EMyFrameworkType::edit>("edtExtentions", os.str());
      }
   catch(std::exception &ex) {
      std::cerr << "error in TProcess::SelectedExtentionsChanged: " << ex.what() << std::endl;
      }
   }



// C++20 format for date time, C++Builder only C++17
void TProcess::ShowFiles(std::ostream& out, fs::path const& strBase, std::vector<fs::path> const& files) {
   #if defined(_MSVC_LANG) // && _MSVC_LANG < 202002L)
   // to_time_t C++17
   // inspiration: https://developercommunity.visualstudio.com/t/stdfilesystemfile-time-type-does-not-allow-easy-co/251213
   // returns loctime ready for use in std::put_time
   static auto filetime_to_localtime = [](fs::path const& p) {
         //magic number in nanoseconds?: 
         auto constexpr __std_fs_file_time_epoch_adjustment = 0x19DB1DED53E8000LL;
         constexpr fs::file_time_type::duration adjustment(__std_fs_file_time_epoch_adjustment);
         static auto toSeconds = [](const auto duration) {
              // divide by 1000000 in chrono-style
              return std::chrono::duration_cast<std::chrono::seconds>(duration);
              };

         auto ftime = std::filesystem::last_write_time(p);
         const auto epoch = ftime.time_since_epoch();
         time_t tt{ toSeconds(epoch - adjustment).count() };
         std::tm loctime;
         localtime_s(&loctime, &tt);
         return loctime;
         };
   #else 
      static auto filetime_to_localtime = [](fs::path const& p) {
      auto ftime = fs::last_write_time(p);
      auto tt = decltype(ftime)::clock::to_time_t(ftime);
      std::tm loctime;
      std::localtime_s(&tt, &loctime);
      return loctime;
   };
#endif

   std::for_each(files.begin(), files.end(), [&out, strBase](auto p) {
      if (fs::is_directory(p)) {
         std::cout << fs::relative(p, strBase) << std::endl;
      }
      else {
         auto loctime = filetime_to_localtime(p);
         out << p.filename().string() << '\t'
            << fs::relative(p.parent_path(), strBase).string() << '\t'
            << std::put_time(&loctime, "%d.%m.%Y %T") << '\t'
            << Convert_Size_KiloByte(fs::file_size(p)) << " KB" << std::endl;
      }
      });
}


void TProcess::CopyFiles(void) {
   TMyToggle toggle("Guard for boActive", boActive);
   std::vector<fs::path> files;
   std::set<std::string> extensions;
   my_formlist<EMyFrameworkType::listbox, std::string> mylist(&frm, "lbValues");
   std::copy(mylist.begin(), mylist.end(), std::inserter(extensions, extensions.end()));
   auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
   if (!strPath) {
      TMyLogger log(__func__, __FILE__, __LINE__);
      log.stream() << "directory to show is empty, set a directory before call this function";
      log.except();
      }
   else {
      auto boSubDir = frm.Get<EMyFrameworkType::checkbox, bool>("chbSubDirs");
      std::chrono::milliseconds time;
      fs::path fsPath = *strPath;
      auto ret = Call(time, Find, std::ref(files), std::cref(fsPath), std::cref(extensions), *boSubDir);
      /*
      std::for_each(files.begin(), files.end(), [](auto const& file) {

         });
      // fs::create_directories
      // copy (from to errc)
      */
      std::clog << " function \"Copy\" procecced in "
         << std::setprecision(3) << time.count() / 1000. << " sec, "
         << files.size() << " files found" << std::endl;

      }
   }

void TProcess::ShowFiles(void) {
   TMyToggle toggle("Guard for boActive", boActive);
   std::vector<fs::path> files;
   std::set<std::string> extensions;
   my_formlist<EMyFrameworkType::listbox, std::string> mylist(&frm, "lbValues");
   //std::copy(mylist.begin(), mylist.end(), std::ostream_iterator<std::string>(std::cerr, "\n"));
   std::copy(mylist.begin(), mylist.end(), std::inserter(extensions, extensions.end()));
   //extensions = { ".mp4", ".svg", ".png", ".bmp" };
   auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
   if (!strPath) {
      TMyLogger log(__func__, __FILE__, __LINE__);
      log.stream() << "directory to show is empty, set a directory before call this function";
      log.except();
      }
   else {
      auto boSubDir = frm.Get<EMyFrameworkType::checkbox, bool>("chbSubDirs");
      std::chrono::milliseconds time;
      fs::path fsPath = *strPath;
      auto ret = Call(time, Find, std::ref(files), std::cref(fsPath), std::cref(extensions), *boSubDir);

      std::clog << " function \"Find\" procecced in "
         << std::setprecision(3) << time.count() / 1000. << " sec, "
         << files.size() << " files found" << std::endl;

      std::sort(files.begin(), files.end(), [](auto lhs, auto rhs) {
         if (auto ret = lhs.filename().string().compare(rhs.filename().string()); ret == 0)
            return lhs.parent_path().string() < rhs.parent_path().string();
         else
            return ret < 0;
         });

      ShowFiles(std::cout, fsPath, files);
      }
   }

/** \brief construction of filename with informations from tplData and base directory
\tparam iFile Contant of int with the position of relative name in tplData
\param base const reference of fs::path with basic path for tplData
\param row const reference of tplData with informations for a information in project file
\return fs::path with the absolute path to the requested file
*/
template <int iFile>
fs::path ConstructFile(fs::path const& base, tplData const& row) {
   return fs::weakly_canonical(base / fs::path(std::get<iMyData_Path>(row)) / fs::path(std::get<iFile>(row)));
}

inline size_t GetFileSize(std::string const& strProject, std::string const& strPath, fs::path const& strFile) {
   try {
      return TMyFileDlg::CheckFileSize(strFile.string());
      }
   catch(my_file_information const& info) {
      std::cerr << "Project: " << strProject << "(" << strPath << ") -> " << info.GetInformation() << std::endl;
      }
   return 0u;
   }

/// method to parse a cbproj file for informations
void TProcess::ParseProject(fs::path const& base, fs::path const& strFile, std::vector<tplData>& projects) {
   auto xml_error = [&strFile](std::ostream& out, pugi::xml_parse_result result) {
               out << "XML [" << strFile.string() << "] parsed with errors" << std::endl
                   << "Error description: " << result.description() << std::endl
                   << "Error offset: " << result.offset << std::endl;
               };
   try {
      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_file(strFile.string().c_str(), pugi::parse_default | pugi::parse_fragment);
      if(!result) {
         TMyLogger log(__func__, __FILE__, __LINE__);
         xml_error(log.stream(), result);
         log.except();
         }

      pugi::xml_node root = doc.document_element();
      pugi::xpath_node xpathNode = root.select_single_node("ItemGroup");
      if(!xpathNode) {
         TMyLogger log(__func__, __FILE__, __LINE__);
         log.stream() << "ItemGroup not found in file " << strFile.string();
         log.except();
         }
      pugi::xml_node selNode = xpathNode.node();
      for(pugi::xml_node child = selNode.child("CppCompile"); child; child = child.next_sibling("CppCompile")) {
         tplData row;
         std::get<iMyData_Project>(row)  = strFile.filename().string();
         std::get<iMyData_Path>(row)     = fs::relative(strFile.parent_path(), base).string();
         std::get<iMyData_Type>(row)     = "Cpp Node";
         std::get<iMyData_Order>(row)    = atoi(child.child_value("BuildOrder"));
         std::get<iMyData_CppFile>(row)  = child.attribute("Include").value();
         std::get<iMyData_H_File>(row)   = child.child_value("DependentOn");
         std::get<iMyData_FrmName>(row)  = child.child_value("Form");
         std::get<iMyData_FrmType>(row)  = child.child_value("FormType");
         std::get<iMyData_FrmClass>(row) = child.child_value("DesignClass");

         if(!std::get<iMyData_CppFile>(row).empty())
            std::get<iMyData_CppRows>(row) = GetFileSize(std::get<iMyData_Project>(row), std::get<iMyData_Path>(row), ConstructFile<iMyData_CppFile>(base, row));

         if(!std::get<iMyData_H_File>(row).empty())
            std::get<iMyData_H_Rows>(row) = GetFileSize(std::get<iMyData_Project>(row), std::get<iMyData_Path>(row), ConstructFile<iMyData_H_File>(base, row));

         if(!std::get<iMyData_FrmName>(row).empty()) {
            std::string strExt;
            if(std::get<iMyData_FrmType>(row).empty()) strExt = ".dfm";   // eventual setting
            else strExt = std::string(".") + std::get<iMyData_FrmType>(row);
            std::get<iMyData_FrmFile>(row) = ( fs::path(std::get<iMyData_CppFile>(row)).parent_path() /
                                               fs::path(std::get<iMyData_CppFile>(row)).stem()).string() +
                                               strExt;
            std::get<iMyData_FrmRows>(row) = GetFileSize(std::get<iMyData_Project>(row), std::get<iMyData_Path>(row), ConstructFile<iMyData_FrmFile>(base, row));
            }

         projects.emplace_back(std::move(row));
         }

      for(pugi::xml_node child = selNode.child("None"); child; child = child.next_sibling("None")) {
         std::string strCurrentFile = child.attribute("Include").value();
         std::string strCurrentExtension = fs::path(strCurrentFile).extension().string();
         if(header_files.find(strCurrentExtension) != header_files.end()) {
            tplData row;
            if(auto it = std::find_if(projects.begin(), projects.end(), [strCurrentFile](auto const& val) {
                  return strCurrentFile == std::get<iMyData_H_File>(val);
                               });it == projects.end()) {
               std::get<iMyData_Project>(row) = strFile.filename().string();
               std::get<iMyData_Path>(row)    = fs::relative(strFile.parent_path(), base).string();
               std::get<iMyData_Type>(row)    = "None Node";
               std::get<iMyData_Order>(row)   = atoi(child.child_value("BuildOrder"));
               std::get<iMyData_H_File>(row)  = strCurrentFile;

               if(!std::get<iMyData_H_File>(row).empty())
                  std::get<iMyData_H_Rows>(row) = GetFileSize(std::get<iMyData_Project>(row), std::get<iMyData_Path>(row), ConstructFile<iMyData_H_File>(base, row));

               projects.emplace_back(std::move(row));
               }
            }
         }

       for(pugi::xml_node child = selNode.child("FormResources"); child; child = child.next_sibling("FormResources")) {
          std::string strCurrentFile = child.attribute("Include").value();
          std::string strCurrentExtension = fs::path(strCurrentFile).extension().string();
          if(form_files.find(strCurrentExtension) != form_files.end()) {
             tplData row;
             if(auto it = std::find_if(projects.begin(), projects.end(), [strCurrentFile](auto const& val) {
                                                                    return strCurrentFile == std::get<iMyData_FrmFile>(val);
                                                                    });it == projects.end()) {
                std::get<iMyData_Project>(row) = strFile.filename().string();
                std::get<iMyData_Path>(row)    = fs::relative(strFile.parent_path(), base).string();
                std::get<iMyData_Type>(row)    = "Form Node";
                std::get<iMyData_Order>(row)   = atoi(child.child_value("BuildOrder"));
                std::get<iMyData_FrmFile>(row) = strCurrentFile;

                if(!std::get<iMyData_FrmFile>(row).empty())
                   std::get<iMyData_FrmRows>(row) = GetFileSize(std::get<iMyData_Project>(row), std::get<iMyData_Path>(row), ConstructFile<iMyData_FrmFile>(base, row));

                projects.emplace_back(std::move(row));
                }
             }
          }
      }
   catch(std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      }
   }

void TProcess::Parse(fs::path const& fsPath, std::vector<fs::path>& project_files, std::vector<tplData>& projects) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = Find(project_files, fsPath, project_extensions, true);

   //for(auto const& file : project_files) ParseProject(fsPath, file, projects);
   /*
   std::for_each(project_files.begin(), project_files.end(), [this, &fsPath, &projects](auto const file) {
                  ParseProject(fsPath, file, projects);
                  }
                  );
   */
   std::for_each(project_files.begin(), project_files.end(), std::bind(&TProcess::ParseProject, this, std::cref(fsPath), std::placeholders::_1, std::ref(projects)));

   std::tuple<size_t, size_t, size_t> rows = { 0u, 0u, 0u };
   std::for_each(projects.begin(), projects.end(), [&rows](auto const& val) {
              std::get<0>(rows) += std::get<iMyData_CppRows>(val);
              std::get<1>(rows) += std::get<iMyData_H_Rows>(val);
              std::get<2>(rows) += std::get<iMyData_FrmRows>(val);
              });

   std::sort(projects.begin(), projects.end(), [](auto lhs, auto rhs) {
      if (auto ret = std::get<iMyData_Project>(lhs).compare(std::get<iMyData_Project>(rhs)); ret == 0) {
         if (auto ret = std::get<iMyData_Path>(lhs).compare(std::get<iMyData_Path>(rhs)); ret == 0) {
            return std::get<iMyData_Order>(lhs) < std::get<iMyData_Order>(rhs);
         }
         else return ret < 0;
      }
      else return ret < 0;
      });

   auto func_ende = std::chrono::high_resolution_clock::now();
   auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);

   TMyLogger log(__func__, __FILE__, __LINE__);
   log.stream().setf(std::ios::fixed);
   log.stream() << project_files.size() << " project(s) processed in " 
                << std::setprecision(3) << time.count() / 1000.
                << " ms, " << projects.size() << " item(s) found, "
                << mySum(rows) << " rows in files (cpp, h, form) ";
   TMyDelimiter<Latin> delimiter = { "(", ", ", ")." };
   myTupleHlp<Latin>::Output(log.stream(), delimiter, rows);
   log.Write(std::clog);

   delimiter = { "", "\t", "\n" };
   std::for_each(projects.begin(), projects.end(), [&delimiter](auto val) {
          myTupleHlp<Latin>::Output(std::cout, delimiter, val);
          });
   }

void TProcess::ParseDirectory(void) {
   std::vector<fs::path> project_files;
   std::vector<tplData> projects;

   auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
   if (!strPath) {
      TMyLogger log(__func__, __FILE__, __LINE__);
      log.stream() << "directory to parse is empty, set a directory before call this function";
      log.except();
      }

   Parse(*strPath, project_files, projects);
}




void TProcess::CountFiles(void) {
   auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
   if (!strPath) {
      TMyLogger log(__func__, __FILE__, __LINE__);
      log.stream() << "directory to parse is empty, set a directory before call this function";
      log.except();
   }
   else {
      auto boSubDir = frm.Get<EMyFrameworkType::checkbox, bool>("chbSubDirs");
      std::chrono::milliseconds time;
      fs::path fsPath = *strPath;
      auto ret = Call(time, Count, std::cref(fsPath), *boSubDir);
      std::get<2>(ret) = Convert_Size_KiloByte(std::get<2>(ret));
      TMyDelimiter<Latin> delimiter = { "", "\t", "\n" };
      myTupleHlp<Latin>::Output(std::cout, delimiter, ret);
      std::clog << "function \"Count\" procecced in "
         << std::setprecision(3) << time.count() / 1000. << " sec" << std::endl;
   }
}


void TProcess::ParseAction() {
   try {
      TMyWait wait;
      showMode = EShowVariante::empty;
      frm.GetAsStream<Latin, EMyFrameworkType::listview>(old_cout, "lvOutput", Project_Columns);
      ParseDirectory();
      showMode = EShowVariante::Projects;
   }
   catch (std::exception& ex) {
      std::cerr << "error in function \"Parse\": " << ex.what() << std::endl;
      std::clog << "error in function \"Parse\"" << std::endl;
   }
}

void TProcess::CopyAction() {

   }

void TProcess::ShowAction() {
   try {
      TMyWait wait;
      showMode = EShowVariante::empty;
      frm.GetAsStream<Latin, EMyFrameworkType::listview>(old_cout, "lvOutput", File_Columns);
      ShowFiles();
      showMode = EShowVariante::Files;
   }
   catch (std::exception& ex) {
      std::cerr << "error in function \"Show\": " << ex.what() << std::endl;
      std::clog << "error in function \"Show\"" << std::endl;
   }
}


void TProcess::CountAction() {
   try {
      // --------------------------------------------------------
      TMyWait wait;
      showMode = EShowVariante::empty;
      frm.GetAsStream<Latin, EMyFrameworkType::listview>(old_cout, "lvOutput", Count_Columns);
      CountFiles();
      showMode = EShowVariante::Count;
   }
   catch (std::exception& ex) {
      std::cerr << "error in function \"Count\": " << ex.what() << std::endl;
      std::clog << "error in function \"Count\"" << std::endl;
   }
}



void TProcess::Open_File(size_t dir, size_t file) {
   try {
      if(GetShowMode() == EShowVariante::Files || GetShowMode() == EShowVariante::Projects) {
         auto strPath = frm.Get<EMyFrameworkType::edit, std::string>("edtDirectory");
         if(strPath) {
            auto selected = frm.GetSelectedRows<EMyFrameworkType::listview>("lvOutput");
            if(selected.size() ==1) {
               auto relpath = frm.GetValue<EMyFrameworkType::listview, std::string>("lvOutput", selected[0], dir);
               auto relfile = frm.GetValue<EMyFrameworkType::listview, std::string>("lvOutput", selected[0], file);
               if(relpath && relfile) {
                  auto file_to_open = fs::weakly_canonical(fs::path(*strPath) / fs::path(*relpath) / fs::path(*relfile));
                  TMyFileDlg::OpenFileAction(file_to_open.string());
                  }
               else {
                  std::cerr << "Can't open file, missing information in selected row" << std::endl;
                  }
               }
            else {
               std::cerr << "open files only with single selected row" << std::endl;
               }
            }
         else {
            std::cerr << "open files only if directory exist" << std::endl;
            }
         }
      else {
         std::cerr << "open files isn't possible in this mode" << std::endl;
         }
      }
   catch(std::exception& ex) {
      std::cerr << "error in function \"Open_File\": " << ex.what() << std::endl;
      std::clog << "error in function \"Open_File\"" << std::endl;
      }

}

void TProcess::CountFileRowsForProjects(std::ostream& out, bool boSelectedOnly) {
   std::vector<size_t> rows;
   if (boSelectedOnly) rows = frm.GetSelectedRows<EMyFrameworkType::listview>("lvOutput");
   else rows = frm.GetAllRows<EMyFrameworkType::listview>("lvOutput");

   std::tuple<size_t, size_t, size_t> cnt = { 0u, 0u, 0u };

   std::for_each(rows.begin(), rows.end(), [&cnt, this](auto row) {
           std::get<0>(cnt) += this->frm.GetValue<EMyFrameworkType::listview, size_t>("lvOutput", row, iMyData_CppRows).value_or(0u);
           std::get<1>(cnt) += this->frm.GetValue<EMyFrameworkType::listview, size_t>("lvOutput", row, iMyData_H_Rows).value_or(0u);
           std::get<2>(cnt) += this->frm.GetValue<EMyFrameworkType::listview, size_t>("lvOutput", row, iMyData_FrmRows).value_or(0u);
           } );
   if (boSelectedOnly) out << "Lines in selected rows (cpp, h, res): ";
   else out << "Lines in all rows (cpp, h, res): ";
   TMyDelimiter<Latin> delimiter{ "(", ", ", ")\n" };
   myTupleHlp<Latin>::Output(out, delimiter, cnt);
   }