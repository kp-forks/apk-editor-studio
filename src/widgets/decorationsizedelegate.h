#ifndef DECORATIONSIZEDELEGATE_H
#define DECORATIONSIZEDELEGATE_H

#include <QStyledItemDelegate>

class DecorationSizeDelegate : public QStyledItemDelegate
{
public:
    explicit DecorationSizeDelegate(const QSize &size, QObject *parent = nullptr);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QSize decorationSize;
};

#endif // DECORATIONSIZEDELEGATE_H
