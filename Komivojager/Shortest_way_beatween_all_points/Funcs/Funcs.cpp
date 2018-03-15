#include "Funcs/Funcs.h"


namespace const_data
{
const char * root                  ("Map"),
           * collect               ("collect"),
           * point                 ("Sity"),
           * name_atribute         ("number"),
           * size_collect          ("size"),
           * value_diagonal        ("0"),
           * filter_for_filedialog ("*.xml"),
           * file_suffix           (".xml"),
           * template_html         ("templates/template_html.html"),
           * file_data             ("folder_for_printer/temp.html"),
           * point_1               ("<!-- #1 -->"),
           * point_2               ("<!-- #2 -->");

}


void funcs::update_table_from_spin( QStandardItemModel * model,int arg1 )
{
    if(!model)
        return;
   auto size = model->rowCount();

    if( size  > arg1 )
    {
       for( auto i = size ; i > arg1; --i )
        {
        model->removeColumn( i - 1 );
        model->removeRow(    i - 1 );
       }
    }else if(  size  < arg1 )
    {
        model->setRowCount(arg1);
        model->setColumnCount( arg1);

    for(int i = size;  i < arg1 ; ++i )
       {
        auto ptr = new QStandardItem( const_data::value_diagonal );
        ptr->setFlags( Qt::ItemIsEditable );
        model->setItem(i,i, ptr );
       }
    }
}


std::vector<std::vector<double>> funcs::fill_matrix_and_revision_model_on_empty_field(const QStandardItemModel &  model,bool & error_model )
{
   std::vector<std::vector<double>> vec( model.rowCount(), std::vector<double>(model.columnCount()) );


   int count_empty_str = 0;
   for( auto size = model.rowCount(), row = 0; row < size; ++row  )
      for( auto column = 0; column < size ; ++column  )
         {
          if(count_empty_str == 2)
          {
              error_model = true;
              goto goto_return; // <----------------------------------------------------- goto
          }
           auto * ptr_item = model.item( row, column);

            if ( ptr_item )
             {

                bool error;

                vec[row][column] = ptr_item->text().toDouble( &error );

                if(!error)
                  {
                  error_model =  true;
                  goto goto_return;  // <----------------------------------------------------- goto
                  }
             }else
                 ++count_empty_str;
         }
      error_model = false;
goto_return: // <----------------------------------------------------- goto
   return std::move(  vec );

}

void funcs::Processing_data::first_pass()
{
    emit started(); // signal

     voyager.first_result();


    // std::sort(result.begin(), result.end());

    emit update_model_result();
    emit finished(); // signal
}

void funcs::Processing_data::full_pass()
{
    emit started();

    if(  voyager.improve_result() )
    emit update_model_result();

    emit finished();
}

void funcs::fill_table_from_matrix( QStandardItemModel & model,const std::vector<std::vector<double>> & matrix)
{
    size_t size = matrix.size();
    model.clear();
    model.blockSignals(true);
     for( size_t row = 0; row < size; ++row)
         for( size_t column = 0; column < size; ++column)
       {

             QStandardItem * ptr = new QStandardItem(  QString::number( matrix[row][column]));
                 if( row == column )
                   ptr->setFlags(Qt::ItemIsEnabled);

                 model.setItem( row, column, ptr );


       }

     model.blockSignals(false);


}
bool funcs::save_all_data_at_file_xml( const std::vector<std::vector<double>> & matrix, QString  directory)
{




    QFile file(directory);
    if( file.open( QIODevice::WriteOnly ) )
    {

        int size = matrix.size();

        QXmlStreamWriter xmlwriter(&file);
        xmlwriter.writeStartDocument();
        xmlwriter.writeStartElement( const_data::root );
        for( int row = 0; row < size; ++row )
        {
           xmlwriter.writeStartElement( const_data::collect );

            for( int column = 0; column < size; ++column )
            {

        //     xmlwriter.writeStartElement( point );
             xmlwriter.writeTextElement( const_data::point , QString::number( matrix[row][column] )  );
          //   xmlwriter.writeEndElement();
            }
            xmlwriter.writeEndElement();
        }
        xmlwriter.writeEndElement();
        xmlwriter.writeEndDocument();

        file.close();

        return true;
    }
    return false;

}

bool funcs::load_data_from_xml_file(QStandardItemModel & model , QSpinBox & spinbox , QString  directory)
{



    QFile file( directory );
    QXmlStreamReader xmlreader(&file);
    std::vector<std::vector<double>> Matrix;
    bool is_fail;

    if( file.open(QIODevice::ReadOnly | QIODevice::Text ) )
       if( QXmlStreamReader::StartDocument ==  xmlreader.readNext() )
         if(  xmlreader.readNextStartElement() )
            {
             if( xmlreader.name() != const_data::root )
                goto goto_return;

             while ( xmlreader.readNextStartElement() )
             {
                if( xmlreader.name() != const_data::collect)
                   goto goto_return;

                 std::vector<double> ar;
                 while ( xmlreader.readNextStartElement() )
                        {
                       if( xmlreader.name() != const_data::point)
                          goto goto_return;
                     else
                        {
                                const auto str = xmlreader.readElementText();
                                double value =  str.toDouble(&is_fail);
                                 if( !is_fail )
                                    goto goto_return;
                                 ar.push_back( value );
                         }

                        }
                 if( !Matrix.empty())
                   if(  Matrix.back().size() != ar.size())
                        goto goto_return;

                 Matrix.push_back( std::move(ar) );

             }
             spinbox.blockSignals(true);
             spinbox.setValue(Matrix.size());
             fill_table_from_matrix( model, Matrix );
             spinbox.blockSignals(false);
             return true;
            }

            goto_return:
            return false;
}

QString funcs::name_directory(QWidget * parent,const QString & name_window ,const QString & DirPath,const QString & filter)
{
    QFileDialog dialog( parent ,name_window, DirPath, filter );
    QStringList list_files;
    QFileInfo info;
    QString str_return;
    bool wrong_suffix = false;

    dialog.setAcceptMode(  QFileDialog::AcceptSave );
    do
    {
      str_return.clear();
      if (  dialog.exec() == QFileDialog::ExistingFile )
         {
           list_files = dialog.selectedFiles();
            info.setFile( list_files.first() );
        // revision suffic file if doesnt include append else revision if wrong repeat
          if( info.suffix().isEmpty() )
             {
              list_files.first().append(  filter  );
                wrong_suffix = false;
             }else if( !info.suffix().compare(filter,Qt::CaseSensitive))
                 wrong_suffix = true;
          str_return = list_files.first();
         }
    }while( wrong_suffix );
      dialog.close();
    return str_return;
}


bool funcs::copy_template_html_and_fill_with_data( const QStandardItemModel & model,const QString & path )
{
      QFile  template_html( const_data::template_html ),
             file_with_way( path );

      if( ! ( template_html.open( QIODevice::ReadOnly) && file_with_way.open( QIODevice::WriteOnly ) ) )
          return false;





    QTextDocument doc(  QTextStream( &template_html ).readAll() );


    QTextCursor cursor = doc.find( const_data::point_1);
    cursor.beginEditBlock();
    cursor.insertText( model.item( model.rowCount() - 1,model.columnCount() - 1)->text() );
    cursor.endEditBlock();
    for( auto count_row = model.rowCount() - 1 , row = 0 ;      row < count_row         ; ++row   )
    {
        QString str("<tr>\n\
                     <td class=\"Counter\"></td>\n\
                     <td  > %1 </td>\n\
                     <td  > %2 </td>\n\
                     <td  > %3 </td>\n\
                     </tr>\n\
                     <!-- #2 -->");


       for( auto count_column = model.columnCount(), column = 0; column < count_column  ; ++column )
       {
            str = str.arg( model.item(row,column)->text() );
       }
       cursor = doc.find( const_data::point_2 );
       cursor.beginEditBlock();
       cursor.insertText( str );
       cursor.endEditBlock();
    }
    file_with_way.write( doc.toPlainText().toStdString().c_str());
    return true;

}
