#include "widgets/iconlist.h"
#include "apk/iconsproxy.h"
#include "base/application.h"
#include "base/debug.h"
#include <QDragEnterEvent>
#include <QMimeData>

IconList::IconList(QWidget *parent) : QListView(parent)
{
    setAcceptDrops(true);
}

void IconList::dragEnterEvent(QDragEnterEvent *event)
{
    // TODO Highlight separate items
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls() || mimeData->hasImage()) {
        event->acceptProposedAction();
        setHighlight(true);
    }
}

void IconList::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    setHighlight(false);
}

void IconList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction(); // Otherwise QListView drag and drop won't work
}

void IconList::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->acceptProposedAction();
        if (model()) {
            QPixmap iconSource(mimeData->urls().at(0).toLocalFile());
            if (!iconSource.isNull()) {
                bool success = false;
                const QModelIndex index = indexAt(event->pos());
                if (index.isValid()) {
                    const QString iconTarget = model()->index(index.row(), IconsProxy::IconPath).data().toString();
                    success = iconSource.save(iconTarget);
                } else {
                    for (int row = 0; row < model()->rowCount(); ++row) {
                        const QString iconTarget = model()->index(row, IconsProxy::IconPath).data().toString();
                        success = iconSource.save(iconTarget);
                    }
                }
                if (success) {
                    emit model()->dataChanged(index, index);
                }
            }
        }
    } else if (mimeData->hasImage()) {
#ifdef QT_DEBUG
        qDebug() << "TODO: Image dropped";
#endif
    }
    setHighlight(false);
}

void IconList::setHighlight(bool value)
{
    QColor background = app->getColor(app->ColorHighlight);
    setStyleSheet(value ? QString("IconList {background: %1}").arg(background.name(QColor::HexArgb)) : QString());
}