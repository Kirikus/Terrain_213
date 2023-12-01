#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <qcustomplot.h>

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
    void on_RLS_widgets_tabCloseRequested(int index);
    void on_add_RLS_clicked();
    void on_apply_button_clicked(QAbstractButton *button);

    void handleResizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow *ui;
    size_t _rls_index = 0;  // index of last RLS in RLS_widget

    QSize _plot_size;

    void _plot_image();
    void _plot_angle_map();
    QSize _calculate_new_plot_size(QCustomPlot* plot);
};

#endif  // MAINWINDOW_H
