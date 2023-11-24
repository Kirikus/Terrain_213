#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_addRLS_clicked();

    void on_RLS_widgets_tabCloseRequested(int index);

    void on_apply_button_clicked(QAbstractButton *button);

private:
    Ui::MainWindow *ui;
    size_t _rls_index = 0;  // index of last RLS in RLS_widget
    std::vector<std::map<std::string, double>> _rls_info; // info about position and parameteres of all RLS

    void _plot_image();
    void _plot_angle_map();
};
#endif  // MAINWINDOW_H
