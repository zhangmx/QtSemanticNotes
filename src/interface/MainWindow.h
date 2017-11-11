#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../models/NoteTreeModel.h"
#include "../models/TagTreeModel.h"

#include "../entities/RootNote.h"

#include <QMainWindow>
#include <QMenu>

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>

#include <QTextDocument>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();



public slots:
    void setupDatabase();
    void setupNotesTree();
    void setupTagsTree();
    void setupNoteModels();

    void loadPossibleLinks();
    QString makeLinks(QString text);

    void chooseModeToEnable(Note* note);

    void on_actionSave_triggered();
    void on_actionViewMode_triggered();

    void on_actionAboutQtSemanticNotes_triggered();
    void on_actionAboutQt_triggered();

    void openNote(Note* note);

    void on_toolButtonAddAlias_clicked();
    void on_toolButtonRemoveAlias_clicked();

    void on_toolButtonAddTag_clicked();
    void on_toolButtonRemoveTag_clicked();

    void on_treeViewNotes_customContextMenuRequested(const QPoint& point);
    void on_treeViewNotes_clicked(const QModelIndex& index);

    void on_treeViewTags_customContextMenuRequested(const QPoint& point);
    void on_treeViewTags_doubleClicked(const QModelIndex& index);

    void on_tableViewSearch_clicked(const QModelIndex& index);

    void setupSearchModel();

    void onContentModified();

    void toggleRemoveTagButtonEnabled();
    void toggleRemoveAliasButtonEnabled();

    void enableViewMode();

    void enableEditMode();

    void addSubnote();

    void renameNote();

    void deleteNote();

private:
    Ui::MainWindow* ui;

    unique_ptr<RootNote> rootNote;
    vector<unique_ptr<Note>> notes;
    QHash<Id, NoteItem*> idToItem;

    NoteItem* noteRootItem;
    TagItem* tagRootItem;

    unique_ptr<NoteTreeModel> noteTreeModel;
    unique_ptr<TagTreeModel> tagTreeModel;

    QString possibleLinks;

    Note* currentNote;

    QTextDocument content;

    unique_ptr<QSqlQueryModel> searchModel;
    unique_ptr<QSqlTableModel> aliasesModel;
    unique_ptr<QSqlRelationalTableModel> tagsModel;
    unique_ptr<QSqlRelationalTableModel> linkedFromModel;
    unique_ptr<QSqlRelationalTableModel> linksToModel;
};

#endif // MAINWINDOW_H
