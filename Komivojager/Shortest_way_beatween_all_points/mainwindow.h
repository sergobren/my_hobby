#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include_libraries.h"
#include <CItem_Delegate_LineEdit/CItem_Delegate_Line_Edit.h>
#include <Funcs/Funcs.h>

namespace Ui {

typedef std::vector< double > vector_double ;


class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals :
    void emit_slot_first_pass_m_data();
    void emit_slot_full_pass_m_data();

private slots:
    void on_spin_size_table_valueChanged(int );
    void on_search_way_button_clicked();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void start_window_pause();
    void stop_window_pause();
    void resizeEvent(QResizeEvent *  );
    void set_thread_signal_for_wait( QThread * );
    void change_managment_table_view();
    void on_back_clicked();
    void show_message_on_message_box( const QString  );
    void update_m_model_result();
    void on_improve_result_clicked();
    void change_visible_consdition_on_conversion();
    void on_print_clicked();
    void on_save_clicked();
    void create_html_file(bool);

private:

    Ui::MainWindow *ui;
    my_delegate::MyDelegate * m_delegate;
    funcs::Processing_data *  m_data;

    QLabel m_label;
    QMovie m_movie;
    QStandardItemModel m_model_data;
    QStandardItemModel m_model_result;

};

#endif // MAINWINDOW_H
