#include "editors/codeeditor.h"
#include "base/application.h"
#include "base/yamlhighlighter.h"
#include "base/xmlhighlighter.h"
#include <QBoxLayout>
#include <QFileIconProvider>
#include <QTextStream>

CodeEditor::CodeEditor(const QString &filename, const QPixmap &icon, QWidget *parent) : BaseEditor(filename, icon, parent)
{
    title = filename.section('/', -2);
    if (icon.isNull()) {
        this->icon = QFileIconProvider().icon(QFileInfo(filename));
    }

    editor = new QPlainTextEdit(this);
    editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    editor->setFont(font);
    editor->setTabStopWidth(4 * QFontMetrics(font).width(' '));

    const QString suffix = QFileInfo(filename).suffix().toLower();
    if (app->formats.extensionsXml().contains(suffix)) {
        syntax = new XmlHighlighter(editor->document());
    } else if (app->formats.extensionsYaml().contains(suffix)) {
        syntax = new YamlHighlighter(editor->document());
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(editor);

    file = new QFile(filename, this);
    load();

    connect(editor, &QPlainTextEdit::textChanged, [=]() {
        setModified(true);
    });
}

bool CodeEditor::load()
{
    file->close();
    if (file->open(QFile::ReadWrite)) {
        QTextStream stream(file);
        stream.setCodec("UTF-8");
        editor->setPlainText(stream.readAll());
        setModified(false);
        return true;
    } else {
        qWarning() << "Error: Could not open code resource file";
        return false;
    }
}

bool CodeEditor::save(const QString &as)
{
    QFile *file = this->file;
    if (!as.isEmpty()) {
        file = new QFile(as, this);
        file->open(QFile::WriteOnly);
    }

    bool result;
    if (file->isWritable()) {
        file->resize(0);
        QTextStream stream(file);
        stream.setCodec("UTF-8");
        stream << editor->toPlainText();
        if (as.isEmpty()) {
            setModified(false);
        }
        result = true;
    } else {
        qWarning() << "Error: Could not save code resource file";
        result = false;
    }

    if (!as.isEmpty()) {
        delete file;
    }
    return result;
}