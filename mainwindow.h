#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_New_triggered();

    void on_Delete_triggered();

    void on_Search_triggered();

    void on_newline_clicked();

    void on_note1_clicked(bool checked);

    void on_note2_clicked(bool checked);

    void on_note4_clicked(bool checked);

    void on_note8_clicked(bool checked);

    void on_pause4_clicked(bool checked);

    void on_pause8_clicked(bool checked);

    void on_unk_clicked(bool notechecked);

    void on_unk_2_clicked(bool notechecked);

    void on_replay_clicked();

    void on_circle_clicked();

    void on_play_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    void removeAllQLabelsRecursively(QWidget *parent);

    void onItemClicked(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
