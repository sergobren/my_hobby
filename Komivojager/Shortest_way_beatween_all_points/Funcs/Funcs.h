#ifndef H_Funks
#define H_Funks

#include "include_libraries.h"

#include <algorithm/m_algorithm.h>

namespace const_data
{
extern   const char * file_data;
extern   const char * filter_for_filedialog;
extern   const char * name_dialog_save;
extern   const char * file_xml_suffix;
}

namespace funcs
{


 // set row and collumn and data by diagonal set 0
 void update_table_from_spin( QStandardItemModel * , int );
 //  return matrix , second parameter set true if model doesnt full
 std::vector<std::vector<double>> fill_matrix_and_revision_model_on_empty_field(const QStandardItemModel & ,bool &  );

 // m_model_data
 void fill_table_from_matrix( QStandardItemModel & , const std::vector<std::vector<double>> & );

 // save tavle in xml fail
 bool save_all_data_at_file_xml(const std::vector<std::vector<double>> & matrix , QString );
 // load table from xml fail
 bool load_data_from_xml_file(  QStandardItemModel & , QSpinBox & , QString );

 QString name_directory(QWidget * parent,const QString & name_window ,const QString & DirPath,const QString & filter, const QString & suffix);

 bool copy_template_html_and_fill_with_data( const QStandardItemModel & model , const QString &);


 class Processing_data : public QObject
 {
  ALG::C_Commis_Voyageur voyager;

   Q_OBJECT
 public:
  explicit Processing_data( const std::vector<std::vector<double>> &  matrix) : voyager(matrix ) { }

  public slots:
   void first_pass();  // isn't full pass but busies the least time
   void full_pass(); // it busies the most time but search the most short way
   std::vector< ALG::S_Way > get_arr_results()
     {
       return voyager.result_way();
     }
 signals :
  void update_model_result();
  void started();
  void finished();
  void exit();



 };
}




#endif // H_Funks
