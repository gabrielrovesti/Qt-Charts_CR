#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapper.h"
#include "nomiclassi.h"
#include "tablewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class TableWidget;
class MainWindow : public QMainWindow, public nomiClassi{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow()=default;
    void readPositionSettings();
protected:
    void createEditMenu();
    void createChartMenu();
    void createMenuBar();
    void createWidgets();
    void createOptions();
    void getChartSettings();
    void updateChartSettings();
    void handleEmptyTable();
    void closeEvent(QCloseEvent *);
    void transitioning();
    void handleEmptyAxis();
private slots:
    void on_line_c_clicked();
    void on_pie_c_clicked();
    void on_bar_c_clicked();
    void go_back_c_clicked();
    void on_candlestick_c_clicked();
    void on_apriJSON_triggered();
    void on_apriXML_triggered();
    void on_esportaJSON_triggered();
    void on_esportaXML_triggered();
    void on_add_row_clicked();
    void on_delete_row_clicked();
    void on_add_column_clicked();
    void on_remove_column_clicked();
    void chartSecondarySettings();
    void whenTableEmpty();
private:
    QGridLayout *mainLayout = new QGridLayout;
    TableWidget *t = nullptr;
    Chart *c = nullptr;
    Ui::MainWindow *ui;
    bool isdonut=false;

    //Inizializzazione variabili grafiche per gestione slot
    QComboBox *m_themeComboBox, *candle_combo_i, *candle_combo_d;
    QCheckBox* m_animationsCheckBox,*m_legendCheckBox, *m_aliasingCheckBox;
    QTextEdit *change_title;
    QPushButton* change_title_button, *change_series_button;
    QLineEdit *add_labels_series, *add_labels_axis;
    QTextEdit* insert_first_value, *insert_first_value_axis, *change_series_candle;
    QMessageBox genericbox;
};
#endif // MAINWINDOW_H
