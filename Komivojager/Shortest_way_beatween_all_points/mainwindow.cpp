#include "mainwindow.h"
#include "ui_mainwindow.h"


namespace Ui
{

const int size_table = 4;
const QString name_gif( "insert_this_folder_into_folder_application_after_compile/gif/giphy.gif" );
double size_window_wait_rate_parent = 0.6;
const char name_window_message_box [] = "Error";
const char table_isnt_full [] = "Fields_arent_full";
const char what_to_do_when_bad_alloc[] = "Doesn't enough memory";
const char name_dialog_open[] = "Open File";
const char name_first_column_model_result[] = "From ->";
const char name_second_column_model_result[] = "<- Where";
const char name_third_column_model_result[] = "Distance";
const char str_total[] = "Total ->";
const char name_progres_dialog[] = "Load File";
const char name_button_abort_dial_load[] = "Abort";
const char name_dialog_save[] = "Save";
const char html_filter_for_dialog [] = "*.html";
const char html_suffix [] = ".html";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_delegate( new my_delegate::MyDelegate(this) ),
    m_data( nullptr ),
    m_label(this),
    m_movie( Ui::name_gif ),
    m_model_data( Ui::size_table ,Ui::size_table)
{
    ui->setupUi(this);
    m_label.setMovie(&m_movie);
    QHBoxLayout * hlayout = new QHBoxLayout( &m_label );
    hlayout->addWidget( new QLabel(QString("Please Wait"),&m_label),0,Qt::AlignHCenter | Qt::AlignVCenter);

    m_label.setLayout( hlayout );
    m_label.setAutoFillBackground(true);

    ui->tableView->setModel( &m_model_data );

    ui->spin_size_table->setMinimum( Ui::size_table );
    ui->spin_size_table->setValue(   Ui::size_table );
    ui->tableView->setItemDelegate(m_delegate);
    ui->tableView->horizontalHeader()->setStretchLastSection( true );
    ui->tableView->setEditTriggers( QAbstractItemView::DoubleClicked );

    ui->management_result_2->hide();
    auto value_geometry = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),qApp->desktop()->availableGeometry());
    this->setGeometry(  value_geometry  );
}

MainWindow::~MainWindow()
{

    delete ui;
    delete m_delegate;
    delete m_data;
}

void MainWindow::on_spin_size_table_valueChanged(int arg1)
{

   funcs::update_table_from_spin( static_cast<QStandardItemModel*>(ui->tableView->model()), arg1 );

}


void MainWindow::on_search_way_button_clicked()
{
     bool error_model;
 try
 {
     std::vector<std::vector<double>> vec = funcs::fill_matrix_and_revision_model_on_empty_field( m_model_data, error_model );  // <--- here can be exeption
     if( !error_model )
       {

         m_data = new funcs::Processing_data( vec ); // <--- here can be exeption
         QThread * thread = new QThread;
         m_data->moveToThread( thread );


          connect(thread,SIGNAL( finished() ),thread,SLOT( deleteLater() ));
          connect(thread,SIGNAL( started() ) ,this,SLOT( change_managment_table_view() ));

          connect(this,SIGNAL(emit_slot_first_pass_m_data()), m_data,SLOT(first_pass()));
          connect(this,SIGNAL(emit_slot_full_pass_m_data()), m_data,SLOT(full_pass()));

          connect(m_data,SIGNAL( started() )  ,this,SLOT( start_window_pause() ));
          connect(m_data,SIGNAL( finished() ) ,this,SLOT( stop_window_pause() ));
          connect(m_data,SIGNAL( update_model_result()) ,this,SLOT( update_m_model_result()) );

       //   connect(m_data,SIGNAL( started())     ,this,SLOT( change_managment_table_view() ));
          connect(m_data,SIGNAL( exit())     ,this,SLOT( change_managment_table_view() ));

          connect(m_data,SIGNAL( exit() ) ,thread,SLOT( quit() ));
          connect(m_data,SIGNAL( exit() ) ,m_data,SLOT( deleteLater() ));

         thread->start();

        emit emit_slot_first_pass_m_data();

       }else
         show_message_on_message_box( Ui::table_isnt_full );

 }catch(const std::bad_alloc & error)
 {
     QString error_info( error.what() );
     error_info += Ui::what_to_do_when_bad_alloc ;
     show_message_on_message_box( error_info );
 }


}


// SAVE
void MainWindow::on_actionSave_triggered()
{

   QString str = funcs::name_directory( this,Ui::name_dialog_save,qApp->applicationDirPath(),const_data::filter_for_filedialog, const_data::file_xml_suffix);

   const QStandardItemModel & model =  static_cast<QStandardItemModel&>( *ui->tableView->model());
   bool error_read_model;

   try
   {
         auto vec = funcs::fill_matrix_and_revision_model_on_empty_field(model, error_read_model);

         if( error_read_model )
         {
             show_message_on_message_box( Ui::table_isnt_full  );
             return;
         }

         QThread * thread = QThread::create( [&vec,str]{ funcs::save_all_data_at_file_xml( vec,str); } );

         set_thread_signal_for_wait(thread);

         thread->start();
   }catch(const std::bad_alloc & error)
   {
       QString error_info( error.what() );
       error_info += Ui::what_to_do_when_bad_alloc ;
       show_message_on_message_box( error_info );
   }

}

// LOAD
void MainWindow::on_actionLoad_triggered()
{

     QString str =  QFileDialog::getOpenFileName(this, Ui::name_dialog_open ,qApp->applicationDirPath(),"*.xml");
     if( !str.isEmpty())
     {
      QThread * thread = QThread::create(
      [this,str] () mutable
      {
         funcs::load_data_from_xml_file( static_cast<QStandardItemModel&>( *this->ui->tableView->model()), * (this->ui->spin_size_table), str);
         this->ui->tableView->update();
      }
      );
      set_thread_signal_for_wait(thread);
      thread->start();
     }

}

void MainWindow::start_window_pause()
{
  this->setDisabled(true);
   m_label.raise();
   m_movie.start();
   m_label.show();

}

void MainWindow::stop_window_pause()
{
 this->setEnabled(true);
 m_movie.stop();
 m_label.hide();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent( event );
    QSize size_window_wait( event->size() * Ui::size_window_wait_rate_parent );

    int x = event->size().width()  /2 - size_window_wait.width() / 2,
        y = event->size().height() /2 - size_window_wait.height() / 2;

    QRect top_left_size ( QPoint(x,y) , size_window_wait  );
    m_movie.setScaledSize( top_left_size.size() );
    m_label.setGeometry( top_left_size  );

}

void MainWindow::set_thread_signal_for_wait(QThread * thread)
{
    connect(thread,SIGNAL(started()),this,SLOT(start_window_pause()));
    connect(thread,SIGNAL(finished()),this,SLOT(stop_window_pause()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
}

void MainWindow::change_managment_table_view()
{
    ui->actionLoad->setEnabled( !ui->actionLoad->isEnabled() );
    ui->actionSave->setEnabled( !ui->actionSave->isEnabled() );

    ui->management_data_2->setVisible( ! ui->management_data_2->isVisible());
    ui->management_result_2->setVisible( ! ui->management_result_2->isVisible());

    // m_data is matrix lengh beatween points and m_result is table how to pass all points
    ui->tableView->model() == & this->m_model_data ? ui->tableView->setModel(&m_model_result) : ui->tableView->setModel(&m_model_data) ;


    if( ui->tableView->editTriggers() == QAbstractItemView::DoubleClicked )
        ui->tableView->setEditTriggers(  QAbstractItemView::NoEditTriggers );
    else
        ui->tableView->setEditTriggers(  QAbstractItemView::DoubleClicked );
}

void MainWindow::on_back_clicked()
{
  // I DONT BLOCK M_DATA WITH MUTEX OR SOMETHING ELSE BECAUSE BUTTON "<-BACK" IS BLOCKED
  // WHEN IN DIFFERENT THREAD CALLS FUNC M_DATA
  emit m_data->exit();   // call func thread->quit join thread_finihed join thread->deleteLater
                    // m_data->deletelater
                   //  mainwindow->change_managment_table_view
  change_visible_consdition_on_conversion();
}

void MainWindow::show_message_on_message_box( const QString message )
{
    QMessageBox::information( this, Ui::name_window_message_box, message );
}

void MainWindow::update_m_model_result()
{
       m_model_result.blockSignals( true );
        m_model_result.clear();
       double sum_steps {};
       for( ALG::S_Way & value : m_data->get_arr_results() )
         {

            QList<QStandardItem *> list_row( {
                                         new QStandardItem( QString::number( value.m_y )),  /* firsr column point from */
                                         new QStandardItem( QString::number( value.m_x )),  /* second column point where */
                                         new QStandardItem( m_model_data.item( value.m_y - 1 ,value.m_x - 1 )->text() ) /* third column point distance */
                                           } );
           sum_steps += list_row.last()->text().toDouble();

           m_model_result.appendRow( list_row );
         }
       m_model_result.appendRow(  { new QStandardItem(),
                                    new QStandardItem( Ui::str_total ),
                                    new QStandardItem( QString::number( sum_steps))
                                  });
       m_model_result.blockSignals( false );
       m_model_result.setHorizontalHeaderLabels(
                                                 QStringList(std::initializer_list<QString> {
                                                     Ui::name_first_column_model_result,
                                                     Ui::name_second_column_model_result,
                                                     Ui::name_third_column_model_result
                                                                                           })
                                                );

}

void MainWindow::on_improve_result_clicked()
{
    emit emit_slot_full_pass_m_data();
    change_visible_consdition_on_conversion();

}

void MainWindow::change_visible_consdition_on_conversion()
{
    QPushButton * button_improve = ui->management_result_2->findChild<QPushButton*>( QString( "improve_result" ));
    button_improve->setVisible( ! button_improve->isVisible() );
}

void MainWindow::on_print_clicked()
{
    this->create_html_file(false);

    QPrinter * printer = new QPrinter;
    QPrintDialog     dialog( printer, this );

    if( dialog.exec() == QPrintDialog::Accepted )
    {
        QProgressDialog prog_dial( Ui::name_progres_dialog,Ui::name_button_abort_dial_load, 0, 100, this );
        QWebEnginePage * page = new QWebEnginePage;

        connect(page, SIGNAL(loadProgress(int)), &prog_dial, SLOT(setValue(int)));
        connect(page, SIGNAL(loadStarted()),     &prog_dial, SLOT(show()));
        connect(page, SIGNAL(loadFinished(bool)),&prog_dial, SLOT(close()));
        QFileInfo info( const_data::file_data);


        QUrl url = QUrl::fromLocalFile( info.absoluteFilePath() );

        page->load( url  );
        connect( page,
                &QWebEnginePage::loadFinished,
                [page,printer,this](bool ok)
        {
              if(ok)
              {
                  page->print(printer,
                              [page,printer,this](bool ok)
                              {
                                          if (!ok)
                                              this->show_message_on_message_box( Ui::name_window_message_box );
                                         delete page;
                                         delete printer;
                              }
                             );
                  return;

               }
                  delete page;
                  delete printer;
             this->show_message_on_message_box(Ui::name_window_message_box);
        }      );

               }
}

void MainWindow::on_save_clicked()
{
    this->create_html_file(true);
}

void MainWindow::create_html_file(bool dialog)
{
    QString str;
    if(dialog)
      str = funcs::name_directory(this,Ui::name_dialog_save,qApp->applicationDirPath(),Ui::html_filter_for_dialog,Ui::html_suffix );
    else
      str = const_data::file_data;
    if(!str.isEmpty())
    if( ! funcs::copy_template_html_and_fill_with_data( this->m_model_result, str ) )
       this->show_message_on_message_box( Ui::name_window_message_box );
}
