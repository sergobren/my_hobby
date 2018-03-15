#include "CItem_Delegate_Line_Edit.h"
 const QRegExp my_delegate::regex ( R"(^[0-9]+[.]?[0-9]*$)" );


my_delegate::MyDelegate::MyDelegate(QObject *parent) :
    QItemDelegate(parent),  validator(regex)
{
}

// TableView need to create an Editor
// Create Editor when we construct MyDelegate
// and return the Editor
QWidget* my_delegate::MyDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &,
                                  const QModelIndex &) const
{
   QLineEdit * editor = new QLineEdit(parent);
   editor->setValidator( &validator );
    return editor;
}

// Then, we set the Editor
// Gets the data from Model and feeds the data to Editor
void my_delegate::MyDelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{

    // Get the value via index of the Model
    auto value = index.model()->data(index, Qt::EditRole).toString();

    // Put the value into the lineedit
    QLineEdit * linedit = static_cast<QLineEdit*>(editor);
    linedit->setText( value );

}

// When we modify data, this model reflect the change
void my_delegate::MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{

   QLineEdit * linedit =  static_cast<QLineEdit*>(editor);
   model->setData(index,linedit->text(),Qt::EditRole);



}

// Give the SpinBox the info on size and location
void my_delegate::MyDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
      QLineEdit * linedit = static_cast<QLineEdit*>(editor);
      linedit->setGeometry( option.rect );
}
