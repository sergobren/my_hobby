#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QLineEdit>
#include <QRegExpValidator>

namespace my_delegate
{

extern const QRegExp regex;

class MyDelegate : public QItemDelegate
{
    Q_OBJECT

    QRegExpValidator validator;

public:
    explicit MyDelegate(QObject *parent = 0);

    // Create Editor when we construct MyDelegate
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Then, we set the Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // When we modify data, this model reflect the change
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    // Give the SpinBox the info on size and location
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};


}
#endif // MYDELEGATE_H
