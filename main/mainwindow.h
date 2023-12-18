#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>

#include <qcustomplot.h>
#include <rls.h>
#include "../lib/point.h"
#include "../lib/models_earth.h"
#include "../lib/reflection_point.h"

#include <iostream>

namespace EM = EarthModels;
namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;
namespace RP = ReflectionPoint;

class AngleMap;

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
    void on_visibility_map_apply_button_clicked(QAbstractButton *button);
    void on_visibility_map_add_RLS_clicked();
    void on_visibility_map_RLS_widgets_tabCloseRequested(int index);

    void on_reflection_apply_button_clicked(QAbstractButton *button);

    void on_stationary_height_apply_button_clicked(QAbstractButton *button);


    void on_dinamic_height_apply_button_clicked(QAbstractButton *button);

    void on_dinamic_height_slider_sliderMoved(int position);

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    Ui::MainWindow *ui;

    EM::ModelFlat fm;
    EM::ModelSpheric spm;
    EM::ModelEffectiveRadius efrm;
    std::vector<EM::ModelEarth*> models{&fm, &spm, &efrm};

    DP::Constant const_dp{4};
    VG::GeoData vegetation;
    EL::GeoData geo_data;
    CD::Constant c{5};

    //------VISIBILITY MAP------------------------------------------------
    size_t visibility_map_rls_index = 0;  // index of last RLS in RLS_widget
    QVector<QVector<QCPCurve*>> visibility_map_rls_contour_curves;  // contours for screening angle map for each rls
    QVector<QColor> visibility_map_colors{QColor("#F2E750"), QColor("#F2B807"), QColor("#F28705"), QColor("#C52104"), QColor("#710301")};  // colors for graphs in angle map
    std::vector<double> visibility_map_angles;  // angles for the i contour (<= angles[i])

    void visibility_map_get_angles();
    void visibility_map_plot_default_map();
    void visibility_map_plot_angle_map(AngleMap angle_map, PointCartesian rls);
    AngleMap visibility_map_screen_angle_search(RLS::Data data);

    //------REFLECTION POINT----------------------------------------------
    void reflection_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map);
    std::complex<double> reflection_find_coefficient(RLS::Data rls_data, Map* map, PointCartesian reflection_point, EarthModels::ModelEarth* model);

    //------STATIONARY HEIGHT DETERMINATION-------------------------------
    void stationary_height_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map);

    //------DINAMIC HEIGHT DETERMINATION-------------------------------
    PointSpheric dinamic_height_object;  // current position of an object
    EM::ModelEarth* dinamic_height_model;
    Map dinamic_height_map{&geo_data, &vegetation, &const_dp, &c};
    RLS::Data dinamic_height_rls_data;
    double dinamic_height_R;
    double dinamic_height_R1;
    double dinamic_height_R2;

    void dinamic_height_plot_graph(PointCartesian rls, PointCartesian target, PointCartesian reflection_point, Map* map);
};

#endif  // MAINWINDOW_H
