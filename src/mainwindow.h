#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    //
    void on_Update_button_clicked();

    void on_Subdivide_button_clicked();

    void on_List_Vertex_itemActivated(QListWidgetItem *item);

    void on_List_Edge_itemActivated(QListWidgetItem *item);

    void on_List_Face_itemActivated(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
