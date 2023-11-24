#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

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
  void on_apply_reset_button_clicked(QAbstractButton *button);
    void on_addRLS_clicked();

  private:
  Ui::MainWindow *ui;
    size_t _rls_index = 0;  // index of last RLS in RLS_widget

};
#endif  // MAINWINDOW_H
