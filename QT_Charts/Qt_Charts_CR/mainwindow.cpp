#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chart.h"
#include "mapper.h"
#include "tablewidget.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    QWidget* mainWidget = new QWidget();
    createMenuBar();
    createChartMenu();
    createEditMenu();
    mainWidget->setLayout(mainLayout);
    mainWidget->adjustSize();
    setCentralWidget(mainWidget);
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "QT Charts", tr("Sei sicuro di voler chiudere?\n"), QMessageBox::No | QMessageBox::Ok);
    if (resBtn != QMessageBox::Ok) {
        event->ignore();
    } else {
        QSettings qsettings("QT Charts");
        qsettings.beginGroup("mainwindow");
        qsettings.setValue("geometry", saveGeometry());
        qsettings.setValue("savestate", saveState());
        qsettings.setValue("maximized", isMaximized());
        if (!isMaximized()) {
            qsettings.setValue("pos", pos());
            qsettings.setValue("size", size());
        }
        qsettings.endGroup();
        event->accept();
    }
}

void MainWindow::readPositionSettings(){
    QSettings qsettings("QT Charts");
    qsettings.beginGroup("mainwindow");

    restoreGeometry(qsettings.value("geometry", saveGeometry()).toByteArray());
    restoreState(qsettings.value("savestate", saveState()).toByteArray());
    move(qsettings.value("pos", pos()).toPoint());
    resize(qsettings.value("size", size()).toSize());
    if (qsettings.value("maximized", isMaximized()).toBool()) showMaximized();

    qsettings.endGroup();
}

void MainWindow::createChartMenu(){
    QWidget* hmenu=new QWidget();
    QHBoxLayout *hlayout = new QHBoxLayout;

    QPushButton *line_b = new QPushButton();
    line_b->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    line_b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QPixmap pixmap_l(":/icons/line_icon.png");
    QIcon line_icon(pixmap_l);
    line_b->setIcon(line_icon);
    line_b->setIconSize(QSize(50, 50));
    QObject::connect(line_b, SIGNAL(clicked()),this, SLOT(on_line_c_clicked()));
    hlayout->addWidget(line_b);

    QPushButton *candlestick_b = new QPushButton();
    candlestick_b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    candlestick_b->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    QPixmap pixmap_c(":/icons/candlestick_icon.png");
    QIcon candle_icon(pixmap_c);
    candlestick_b->setIcon(candle_icon);
    candlestick_b->setIconSize(QSize(50, 50));
    QObject::connect(candlestick_b, SIGNAL(clicked()),this, SLOT(on_candlestick_c_clicked()));
    hlayout->addWidget(candlestick_b);

    QPushButton *pie_b = new QPushButton();
    pie_b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pie_b->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    QPixmap pixmap_p(":/icons/pie_icon.png");
    QIcon pie_icon(pixmap_p);
    pie_b->setIcon(pie_icon);
    pie_b->setIconSize(QSize(50, 50));
    QObject::connect(pie_b, SIGNAL(clicked()),this, SLOT(on_pie_c_clicked()));
    connect(pie_b, &QCheckBox::toggled, this, &MainWindow::updateChartSettings);
    hlayout->addWidget(pie_b);

    QPushButton *bar_b = new QPushButton();
    QPixmap pixmap_b(":/icons/bar_icon.png");
    QIcon bar_icon(pixmap_b);
    bar_b->setIcon(bar_icon);
    bar_b->setIconSize(QSize(50, 50));
    bar_b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    bar_b->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    QObject::connect(bar_b, SIGNAL(clicked()),this, SLOT(on_bar_c_clicked()));
    hlayout->addWidget(bar_b);

    QPushButton *back_button = new QPushButton();
    QPixmap pixmap_back(":/icons/back_icon.png");
    QIcon back_icon(pixmap_back);
    back_button->setIcon(back_icon);
    back_button->setIconSize(QSize(50, 50));
    back_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    back_button->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    QObject::connect(back_button, SIGNAL(clicked()),this, SLOT(go_back_c_clicked()));
    hlayout->addWidget(back_button);

    hmenu->setLayout(hlayout);
    hmenu->adjustSize();
    mainLayout->addWidget(hmenu, 0, 0, Qt::AlignTop);
}

void MainWindow::createEditMenu(){
    QFormLayout *vlayout = new QFormLayout;

    if(dynamic_cast<line*>(c) || dynamic_cast<bar*>(c)){
        QPushButton *add_column = new QPushButton;
        QPixmap pixmap_add(":/icons/add_column.png");
        QIcon add_icon(pixmap_add);
        add_column->setIcon(add_icon);
        add_column->setIconSize(QSize(50, 50));
        add_column->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        add_column->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) add_column->setEnabled(false);
        QObject::connect(add_column, SIGNAL(clicked()),this, SLOT(on_add_column_clicked()));

        add_labels_series=new QLineEdit;
        add_labels_series->setGeometry(QRect(QPoint(100, 100), QSize(100, 100)));
        add_labels_series->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(t->getChartDisplayed()->getSeries().size()==0) add_labels_series->setReadOnly(true);

        vlayout->addRow(add_column, add_labels_series);

        QPushButton *delete_column = new QPushButton;
        QPixmap pixmap_delete(":/icons/remove_column.png");
        QIcon delete_icon(pixmap_delete);
        delete_column->setIcon(delete_icon);
        delete_column->setIconSize(QSize(50, 50));
        delete_column->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        delete_column->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) delete_column->setEnabled(false);
        QObject::connect(delete_column, SIGNAL(clicked()),this, SLOT(on_remove_column_clicked()));
        vlayout->addRow(delete_column);

        QPushButton *add_row = new QPushButton;
        QPixmap pixmap_row(":/icons/add_row.png");
        QIcon add_row_icon(pixmap_row);
        add_row->setIcon(add_row_icon);
        add_row->setIconSize(QSize(50, 50));
        add_row->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        add_row->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) add_row->setEnabled(false);
        QObject::connect(add_row, SIGNAL(clicked()),this, SLOT(on_add_row_clicked()));

        add_labels_axis=new QLineEdit;
        add_labels_axis->setGeometry(QRect(QPoint(100, 100), QSize(100, 100)));
        add_labels_axis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(t->getChartDisplayed()->getSeries().size()==0) add_labels_axis->setReadOnly(true);
        vlayout->addRow(add_row, add_labels_axis);

        QPushButton *delete_row = new QPushButton;
        QPixmap pixmap_delete_row(":/icons/remove_row.png");
        QIcon delete_row_icon(pixmap_delete_row);
        delete_row->setIcon(delete_row_icon);
        delete_row->setIconSize(QSize(50, 50));
        delete_row->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        delete_row->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) delete_row->setEnabled(false);
        QObject::connect(delete_row, SIGNAL(clicked()),this, SLOT(on_delete_row_clicked()));
        vlayout->addRow(delete_row);
    }
    if(dynamic_cast<CandleStick*>(c) || dynamic_cast<pie*>(c)){
        QPushButton *add_column = new QPushButton;
        QPixmap pixmap_add(":/icons/add_column.png");
        QIcon add_icon(pixmap_add);
        add_column->setIcon(add_icon);
        add_column->setIconSize(QSize(50, 50));
        add_column->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        add_column->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) add_column->setEnabled(false);
        QObject::connect(add_column, SIGNAL(clicked()),this, SLOT(on_add_column_clicked()));

        add_labels_series=new QLineEdit;
        add_labels_series->setGeometry(QRect(QPoint(100, 100), QSize(100, 100)));
        add_labels_series->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) add_labels_series->setReadOnly(true);
        vlayout->addRow(add_column, add_labels_series);

        QPushButton *delete_column = new QPushButton;
        QPixmap pixmap_delete(":/icons/remove_column.png");
        QIcon delete_icon(pixmap_delete);
        delete_column->setIcon(delete_icon);
        delete_column->setIconSize(QSize(50, 50));
        delete_column->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        delete_column->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
        if(t->getChartDisplayed()->getSeries().size()==0 || t->getMapper()->rowCount()==0|| t->getMapper()->columnCount()==0) delete_column->setEnabled(false);
        QObject::connect(delete_column, SIGNAL(clicked()),this, SLOT(on_remove_column_clicked()));
        vlayout->addRow(delete_column);
    }

    QGroupBox *editSettings = new QGroupBox("Opzioni modifica");
    editSettings->setLayout(vlayout);
    editSettings->adjustSize();
    mainLayout->addWidget(editSettings, 2, 0);
}
void MainWindow::createMenuBar(){
    QMenuBar* menuBar = new QMenuBar;
    QMenu* import_menu = new QMenu(tr("&Importa"), this);
    QMenu* export_menu = new QMenu(tr("&Esporta"), this);

    QAction *importXML= new QAction("&XML", this);
    importXML->connect(importXML,&QAction::triggered,this, &MainWindow::on_apriXML_triggered);
    QAction *importJSON= new QAction("&JSON", this);
    importJSON->connect(importJSON,&QAction::triggered,this, &MainWindow::on_apriJSON_triggered);

    import_menu->addAction(importXML);
    import_menu->addAction(importJSON);

    QAction *exportXML= new QAction("&XML", this);
    exportXML->connect(exportXML,&QAction::triggered,this, &MainWindow::on_esportaXML_triggered);
    QAction *exportJSON= new QAction("&JSON", this);
    exportJSON->connect(exportJSON,&QAction::triggered,this, &MainWindow::on_esportaJSON_triggered);
    export_menu->addAction(exportXML);
    export_menu->addAction(exportJSON);

    menuBar->addMenu(import_menu);
    menuBar->addMenu(export_menu);
    menuBar->adjustSize();
    this->setMenuBar(menuBar);
}

void MainWindow::handleEmptyTable(){
    QFormLayout *layout_empty=new QFormLayout();

    QPushButton* insert=new QPushButton("Inserisci");
    insert->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    insert->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QObject::connect(insert, SIGNAL(clicked()),this, SLOT(whenTableEmpty()));

    insert_first_value=new QTextEdit;
    insert_first_value->setMaximumSize(QSize(1000, 50));
    insert_first_value->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    insert_first_value->setPlaceholderText("Inserisci prima Label");

    insert_first_value_axis=new QTextEdit;
    insert_first_value_axis->setMaximumSize(QSize(1000, 50));
    insert_first_value_axis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    insert_first_value_axis->setPlaceholderText("Inserisci Asse X");

    layout_empty->addRow(insert_first_value);
    layout_empty->addRow(insert_first_value_axis);
    layout_empty->addRow(insert);

    QGroupBox *first_edit_settings = new QGroupBox("Prima impostazione");
    first_edit_settings->setLayout(layout_empty);
    first_edit_settings->adjustSize();
    mainLayout->addWidget(first_edit_settings, 0, 1);
}


void MainWindow::whenTableEmpty(){
    if(insert_first_value->toPlainText() != nullptr){
        QString input=insert_first_value->toPlainText();
        if(dynamic_cast<CandleStick*>(t->getChartDisplayed())){
            if(!input.toInt()){
                genericbox.setText("Inserisci un numero nella serie, non una stringa");
                genericbox.exec();
            }
            else{
                c->clean();

                c = new CandleStick("Candlestick Chart");
                t = new TableWidget(c);

                t->getChartDisplayed()->setAxis({input});
                QHash<QString, QStringList> value;
                value["asseX"].append(input);
                c->setOption(c, value);

                c->setRangeAsseY(1, 10);
                c->insert(input, {"0", "0", "0", "0"});

                t->createChartDisplayed();
                createMenuBar();
                createChartMenu();
                createEditMenu();
                createWidgets();
                createOptions();
                updateChartSettings();
            }
        }
        if(dynamic_cast<bar*>(t->getChartDisplayed())){
            c->clean();
            c = new bar("Bar Chart");
            t = new TableWidget(c);

            c->setAxis({"A", "B", "C", "D"}, 0, 20, "green");

            if(insert_first_value_axis != nullptr){
                QStringList attualeAsseX = t->getChartDisplayed()->getAxisX();
                attualeAsseX.append(insert_first_value_axis->toPlainText());
                t->getChartDisplayed()->setAxis(attualeAsseX);

                QHash<QString, QStringList> value;
                value["asseX"].append(attualeAsseX);
                c->setOption(c, value);
            }

            c->insert(input, {"0"});
            t->getChartDisplayed()->setRangeAsseY(1, 10);
            t->createChartDisplayed();

            for(int i=0; i<c->getAxisX().size()-1; i++){
                t->getMapper()->insertRow(t->getMapper()->rowCount()-1, 1, QModelIndex());
                t->setSeriesMapper(t->getMapper()->getOperationType());
            }

            createMenuBar();
            createChartMenu();
            createEditMenu();
            createWidgets();
            createOptions();
            updateChartSettings();
        }
        if(dynamic_cast<pie*>(t->getChartDisplayed())){
            c->clean();

            QMessageBox::StandardButton choice;
            choice = QMessageBox::question(this, "Imposta grafico a torta", "Vuoi settare la ciambella?",QMessageBox::Ok|QMessageBox::No);
            if (choice == QMessageBox::Ok) {
              c = new pie("Donut Chart", true);
              isdonut=true;
            } else {
                c = new pie("Pie Chart", false);
                isdonut=false;
            }
            t = new TableWidget(c);

            c->setRangeAsseY(1, 10);
            c->insert(input, {"0"});

            t->createChartDisplayed();
            createMenuBar();
            createChartMenu();
            createEditMenu();
            createOptions();
            createWidgets();
            updateChartSettings();
        }
        if(dynamic_cast<line*>(t->getChartDisplayed())){
            c->clean();

            c = new line("Line Chart");
            t = new TableWidget(c);

            c->setAxis({"A", "B", "C", "D"}, 0, 20, "green");
            c->setRangeAsseY(1, 10);
            if(insert_first_value_axis != nullptr){
                QStringList attualeAsseX = t->getChartDisplayed()->getAxisX();
                attualeAsseX.append(insert_first_value_axis->toPlainText());
                t->getChartDisplayed()->setAxis(attualeAsseX);

                QHash<QString, QStringList> value;
                value["asseX"].append(attualeAsseX);
                c->setOption(c, value);
            }

            c->insert(input, {"0"});
            t->createChartDisplayed();

            for(int i=0; i<c->getAxisX().size()-1; i++){
                t->getMapper()->insertRow(t->getMapper()->rowCount()-1, 1, QModelIndex());
                t->setSeriesMapper(t->getMapper()->getOperationType());
            }

            createMenuBar();
            createChartMenu();
            createEditMenu();
            createWidgets();
            createOptions();
            updateChartSettings();
        }
    }
}

void MainWindow::createWidgets(){
    if(t->getChartDisplayed()->getSeries().size() == 0 ){
        handleEmptyTable();
    }
    else if(t->getMapper()->rowCount()==0 || t->getMapper()->columnCount()==0){
        genericbox.setText("Inserisci un valore per riempire la tabella");
        genericbox.exec();
        QWidget* table=mainLayout->itemAtPosition(1, 0)->widget();
        QWidget* edit=mainLayout->itemAtPosition(2, 0)->widget();
        if(edit != nullptr){
            mainLayout->removeWidget(edit);
            edit->hide();
            createEditMenu();
        }
        if(table != nullptr){
            mainLayout->removeWidget(table);
            table->deleteLater();
        }
        handleEmptyTable();
        mainLayout->addWidget(t->getChartDisplayedWidget(), 1, 1);
    }
    else{
        if(c->getSeries().size() > 0){
            QLayoutItem *item = mainLayout->itemAtPosition(0, 1);
            if(item) {
                QWidget* wid=item->widget();
                if(wid != nullptr) delete wid;
            }
        }
        mainLayout->addWidget(t->getTableWidget(), 1, 0);
        mainLayout->addWidget(t->getChartDisplayedWidget(), 1, 1);
        mainLayout->setColumnStretch(1, 1);
        mainLayout->setColumnStretch(1, 1);
    }
}

void MainWindow::createOptions(){
    QFormLayout *chartSettingsLayout = new QFormLayout();

    change_title=new QTextEdit;
    change_title->setMaximumSize(QSize(1000, 50));
    change_title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    change_title_button=new QPushButton("Cambia titolo");
    change_title_button->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    change_title_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(change_title_button, SIGNAL(clicked()), this, SLOT(chartSecondarySettings()));

    m_themeComboBox=new QComboBox;
    m_themeComboBox->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    m_themeComboBox->addItem("Chiaro", QChart::ChartThemeLight);
    m_themeComboBox->addItem("Ceruleo", QChart::ChartThemeBlueCerulean);
    m_themeComboBox->addItem("Scuro", QChart::ChartThemeDark);
    m_themeComboBox->addItem("Sabbia", QChart::ChartThemeBrownSand);
    m_themeComboBox->addItem("Blu scuro", QChart::ChartThemeBlueNcs);
    m_themeComboBox->addItem("Alto contrasto", QChart::ChartThemeHighContrast);
    m_themeComboBox->addItem("Ghiaccio", QChart::ChartThemeBlueIcy);
    m_themeComboBox->addItem("Qt Theme", QChart::ChartThemeQt);
    m_themeComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(m_themeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this, &MainWindow::updateChartSettings);

    m_animationsCheckBox=new QCheckBox;
    m_animationsCheckBox->setCheckState(Qt::Unchecked);
    m_animationsCheckBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(m_animationsCheckBox, &QCheckBox::toggled, this, &MainWindow::updateChartSettings);

    m_aliasingCheckBox=new QCheckBox;
    m_aliasingCheckBox->setCheckState(Qt::Unchecked);
    m_aliasingCheckBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(m_aliasingCheckBox, &QCheckBox::toggled, this, &MainWindow::updateChartSettings);

    m_legendCheckBox=new QCheckBox;
    m_legendCheckBox->setCheckState(Qt::Unchecked);
    m_legendCheckBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(m_legendCheckBox, &QCheckBox::toggled, this, &MainWindow::updateChartSettings);

    chartSettingsLayout->addRow(change_title, change_title_button);
    if(c->getSeries().size() > 0){
        chartSettingsLayout->addRow("Tema", m_themeComboBox);
        chartSettingsLayout->addRow("Animazioni", m_animationsCheckBox);
        chartSettingsLayout->addRow("Antialiasing", m_aliasingCheckBox);
        chartSettingsLayout->addRow("Legenda", m_legendCheckBox);
    }

    candle_combo_i=new QComboBox;
    candle_combo_i->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    candle_combo_i->addItem("Blu", 0);
    candle_combo_i->setItemData( 0, QColor( Qt::blue ), Qt::BackgroundRole );
    candle_combo_i->addItem("Rosso", 1);
    candle_combo_i->setItemData( 1, QColor( Qt::red ), Qt::BackgroundRole );
    candle_combo_i->addItem("Verde", 2);
    candle_combo_i->setItemData( 2, QColor( Qt::green ), Qt::BackgroundRole );
    candle_combo_i->addItem("Giallo", 3);
    candle_combo_i->setItemData( 3, QColor( Qt::yellow ), Qt::BackgroundRole );
    candle_combo_i->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(candle_combo_i, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this, &MainWindow::chartSecondarySettings);
    if(c->getSeries().size() > 0){
         if(dynamic_cast<CandleStick*>(c)) chartSettingsLayout->addRow("Colore serie superiore", candle_combo_i);
    }

    candle_combo_d=new QComboBox;
    candle_combo_d->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    candle_combo_d->addItem("Blu", 0);
    candle_combo_d->setItemData( 0, QColor( Qt::blue ), Qt::BackgroundRole );
    candle_combo_d->addItem("Rosso", 1);
    candle_combo_d->setItemData( 1, QColor( Qt::red ), Qt::BackgroundRole );
    candle_combo_d->addItem("Verde", 2);
    candle_combo_d->setItemData( 2, QColor( Qt::green ), Qt::BackgroundRole );
    candle_combo_d->addItem("Giallo", 3);
    candle_combo_d->setItemData( 3, QColor( Qt::yellow ), Qt::BackgroundRole );
    candle_combo_d->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(candle_combo_d, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this, &MainWindow::chartSecondarySettings);

    if(c->getSeries().size() > 0){
        if(dynamic_cast<CandleStick*>(c)) chartSettingsLayout->addRow("Colore serie inferiore", candle_combo_d);
    }

    change_series_candle=new QTextEdit();
    change_series_candle->setMaximumSize(QSize(1000, 50));
    change_series_candle->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    change_series_candle->setPlaceholderText("Setting serie");

    change_series_button=new QPushButton("Cambia nome serie");
    change_series_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    change_series_button->setGeometry(QRect(QPoint(50, 50), QSize(50, 50)));
    connect(change_series_button,  SIGNAL(clicked()), this, SLOT(chartSecondarySettings()));

    if(c->getSeries().size() > 0){
        if(dynamic_cast<CandleStick*>(c)) chartSettingsLayout->addRow(change_series_candle, change_series_button);
    }

    QGroupBox *chartSettings = new QGroupBox("Opzioni grafico");
    chartSettings->setLayout(chartSettingsLayout);
    chartSettings->adjustSize();
    mainLayout->addWidget(chartSettings, 2, 1);
}

void MainWindow::chartSecondarySettings(){
    if(c){
        if(change_title->toPlainText() != nullptr){
            c->getGrafico()->setTitle(change_title->toPlainText());
            c->setNomeGrafico(change_title->toPlainText());
            c->salvaGrafico(c);
            t->getChartDisplayedWidget()->update();
            change_title->clear();
        }
        else{
            genericbox.setText("Inserisci un valore per cambiare nome");
            genericbox.exec();
        }

        if(dynamic_cast<CandleStick*>(c)){
            if(change_series_candle->toPlainText() != nullptr){
                if(!c->getSeries().isEmpty()){
                    c->getSeries().at(0)->setName(change_series_candle->toPlainText());
                    t->getChartDisplayedWidget()->update();
                }
                else if(c->getSeries().isEmpty()){
                    genericbox.setText("Inserisci un valore nella serie per cambiare nome");
                    genericbox.exec();
                }
            }
            if(!c->getSeries().isEmpty()){
                int color_series_i=0, color_series_d=0;
                color_series_i = candle_combo_i->itemData(candle_combo_i->currentIndex()).toInt();
                QColor custom_c_i, custom_c_d;
                if(color_series_i==0) custom_c_i=Qt::blue;
                if(color_series_i==1) custom_c_i=Qt::red;
                if(color_series_i==2) custom_c_i=Qt::green;
                if(color_series_i==3) custom_c_i=Qt::yellow;
                dynamic_cast<QCandlestickSeries*>(c->getSeries().at(0))->setIncreasingColor(custom_c_i);

                color_series_d = candle_combo_d->itemData(candle_combo_d->currentIndex()).toInt();
                if(color_series_d==0) custom_c_d=Qt::blue;
                if(color_series_d==1) custom_c_d=Qt::red;
                if(color_series_d==2) custom_c_d=Qt::green;
                if(color_series_d==3) custom_c_d=Qt::yellow;
                dynamic_cast<QCandlestickSeries*>(c->getSeries().at(0))->setDecreasingColor(custom_c_d);

                t->getChartDisplayedWidget()->update();
            }
            else{
                genericbox.setText("Inserisci un valore nella serie per cambiare colore");
                genericbox.exec();
            }
        }
    }
    else{
        genericbox.setText("Seleziona un grafico per poter continuare");
        genericbox.exec();
    }
}

void MainWindow::updateChartSettings(){
    if(c){
        QChart::ChartTheme theme;
        theme = static_cast<QChart::ChartTheme>(m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt());
        t->getChartDisplayedWidget()->chart()->setTheme(theme);

        t->getChartDisplayedWidget()->setRenderHint(QPainter::Antialiasing, m_aliasingCheckBox->isChecked());
        m_animationsCheckBox->checkState()==Qt::Checked ? t->getChartDisplayedWidget()->chart()->setAnimationOptions(QChart::AllAnimations) : t->getChartDisplayedWidget()->chart()->setAnimationOptions(QChart::NoAnimation);
        m_legendCheckBox->checkState()==Qt::Checked ? t->getChartDisplayedWidget()->chart()->legend()->show() : t->getChartDisplayedWidget()->chart()->legend()->hide();

        QHash<QString, QStringList> value;
        value["antialiasing"].append(m_aliasingCheckBox->checkState()==Qt::Checked ? "1" : "0");
        value["animazioni"].append(m_animationsCheckBox->checkState()==Qt::Checked ? "1" : "0");
        value["mostralegenda"].append(m_legendCheckBox->checkState()==Qt::Checked ? "1" : "0");
        value["tema"].append(m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toString());
        value["asseX"].append(t->getChartDisplayed()->getAxisX());
        value["isCiambella"].append((isdonut) ? "1" : "0");

        c->setOption(c, value);
    }
    else{
        if(m_animationsCheckBox->checkState()==Qt::Checked) m_animationsCheckBox->setChecked(false);
        if(m_legendCheckBox->checkState()==Qt::Checked) m_legendCheckBox->setChecked(false);
        if(m_aliasingCheckBox->checkState()==Qt::Checked) m_aliasingCheckBox->setChecked(false);
        genericbox.setText("Seleziona un grafico per continuare");
        genericbox.exec();
    }
}

void MainWindow::go_back_c_clicked(){
    this->hide();
    c->clean();
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setWindowTitle(QApplication::translate("qtcharts", "QT Charts"));
    mainWindow->setMinimumSize(800, 800);
    mainWindow->readPositionSettings();
    mainWindow->show();
}

void MainWindow::transitioning(){
    if(c) delete c;
    if (mainLayout) {
     while(mainLayout->count() > 0){
         QLayoutItem *item = mainLayout->takeAt(0);
         QWidget* widget = item->widget();
         if(widget != nullptr)
             delete widget;
         delete item;
     }
   }
}

void MainWindow::getChartSettings(){
    if(!t->getChartDisplayed()->getOptions()[t->getChartDisplayed()->getTipoGrafico(c)].isEmpty()){
       auto op = t->getChartDisplayed()->getOptions();

       QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(op[t->getChartDisplayed()->getTipoGrafico(c)].value("tema")[0].toInt());

       t->getChartDisplayedWidget()->chart()->setTheme(theme);
       m_themeComboBox->setCurrentIndex(op[t->getChartDisplayed()->getTipoGrafico(c)].value("tema")[0].toInt());

       if(op[t->getChartDisplayed()->getTipoGrafico(c)].value("antialiasing")[0] == "1"){
           t->getChartDisplayedWidget()->setRenderHint(QPainter::Antialiasing, true);
           m_aliasingCheckBox->setChecked(true);
       }
       else{
           t->getChartDisplayedWidget()->setRenderHint(QPainter::Antialiasing, false);
           m_aliasingCheckBox->setChecked(false);
       }

       if(op[t->getChartDisplayed()->getTipoGrafico(c)].value("animazioni")[0] == "1"){
           t->getChartDisplayedWidget()->chart()->setAnimationOptions(QChart::AllAnimations);
           m_animationsCheckBox->setChecked(true);
       }
       else{
           t->getChartDisplayedWidget()->chart()->setAnimationOptions(QChart::NoAnimation);
           m_animationsCheckBox->setChecked(false);
       }

       if(op[t->getChartDisplayed()->getTipoGrafico(c)].value("mostralegenda")[0] == "1"){
           t->getChartDisplayedWidget()->chart()->legend()->show();
           m_legendCheckBox->setChecked(true);
       }
       else{
           t->getChartDisplayedWidget()->chart()->legend()->hide();
           m_legendCheckBox->setChecked(false);
       }

       if(!op[t->getChartDisplayed()->getTipoGrafico(c)].value("asseX").isEmpty()){
           t->getChartDisplayed()->setAxis(op[t->getChartDisplayed()->getTipoGrafico(c)].value("asseX"));
       }

       if(op[t->getChartDisplayed()->getTipoGrafico(c)].value("isCiambella")[0] == "1"){
            isdonut=true;
       }
       else{
            isdonut=false;
       }
   }
}

void MainWindow::on_line_c_clicked(){
    if(!c->isEmpty()) transitioning();

    c = new line("Line Chart");
    t = new TableWidget(c);
    t->getChartDisplayed()->pulisciSerie();

    if(!t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c)).isEmpty()){
           QHash<QString, QStringList> gr = t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c));
           QHash<QString, QStringList> op = t->getChartDisplayed()->getOptions().value(t->getChartDisplayed()->getTipoGrafico(c));
           for(int i=0; i<gr.value("labels").size(); i++){
               QStringList aa;
               int div = gr.value("values").size()/gr.value("labels").size();
               for(int j=0; j<div; j++){
                   aa.append(gr.value("values")[(div*i)+j]);
               }
               c->insert(gr.value("labels")[i], aa);
           }
           c->setAxis(op.value("asseX"));
    }
    else{
         //c->insert("0", {"0", "0", "0", "0"});
    }
    t->createChartDisplayed();
    createMenuBar();
    createChartMenu();
    createWidgets();
    createEditMenu();
    createOptions();
    getChartSettings();
    updateChartSettings();
}

void MainWindow::on_pie_c_clicked(){
    if(!c->isEmpty()) transitioning();

    if (isdonut) {
        c = new pie("Donut Chart", true);

    } else {
        c = new pie("Pie Chart", false);
    }

    t = new TableWidget(c);
    t->getChartDisplayed()->pulisciSerie();

    if(!t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c)).isEmpty()){
        QHash<QString, QStringList> gr = t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c));
        QHash<QString, QStringList> op = t->getChartDisplayed()->getOptions().value(t->getChartDisplayed()->getTipoGrafico(c));
        if(!op.value("isCiambella").isEmpty()){
            dynamic_cast<pie*>(c)->setDonut(true);
        }
        for(int i=0; i<gr.value("labels").size(); i++){
            QStringList aa;
            int div = gr.value("values").size()/gr.value("labels").size();
            for(int j=0; j<div; j++){
                aa.append(gr.value("values")[(div*i)+j]);
            }
            c->insert(gr.value("labels")[i], aa);
        }
        if(!op.value("asseX").isEmpty()){
            c->setAxis(op.value("asseX"));
        }
    }
    else{
        //c->insert("0", {"0"});
    }
    t->createChartDisplayed();
    createMenuBar();
    createChartMenu();
    createOptions();
    createWidgets();
    createEditMenu();

    getChartSettings();
    updateChartSettings();
}

void MainWindow::on_bar_c_clicked(){
    if(!c->isEmpty()) transitioning();

    c = new bar("Bar Chart");
    t = new TableWidget(c);
    t->getChartDisplayed()->pulisciSerie();

    if(!t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c)).isEmpty()){
        QHash<QString, QStringList> gr = t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c));
        QHash<QString, QStringList> op = t->getChartDisplayed()->getOptions().value(t->getChartDisplayed()->getTipoGrafico(c));
        for(int i=0; i<gr.value("labels").size(); i++){
            QStringList aa;
            int div = gr.value("values").size()/gr.value("labels").size();
            for(int j=0; j<div; j++){
                aa.append(gr.value("values")[(div*i)+j]);
            }
            c->insert(gr.value("labels")[i], aa);
        }
        c->setAxis(op.value("asseX"));
    }
    else{
        //c->insert("0", {"0"});
    }

    t->createChartDisplayed();
    createMenuBar();
    createChartMenu();
    createWidgets();
    createEditMenu();
    createOptions();

    getChartSettings();
    updateChartSettings();
}

void MainWindow::on_candlestick_c_clicked(){
    if(!c->isEmpty()) transitioning();

    c = new CandleStick("Candlestick Chart");
    t = new TableWidget(c);
    t->getChartDisplayed()->pulisciSerie();

    if(!t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c)).isEmpty()){
        QHash<QString, QStringList> gr = t->getChartDisplayed()->getGraficiArray().value(t->getChartDisplayed()->getTipoGrafico(c));
        QHash<QString, QStringList> op = t->getChartDisplayed()->getOptions().value(t->getChartDisplayed()->getTipoGrafico(c));
        for(int i=0; i<gr.value("labels").size(); i++){
            QStringList aa;
            for(int j=1; j<gr.value("values").size()/gr.value("labels").size(); j++){
                aa.append(gr.value("values")[(5*i)+j]);
            }
            c->insert(gr.value("labels")[i], aa);
        }
        c->setAxis(op.value("asseX"));
    }
    else{
        //c->insert("0", {"0", "0", "0", "0"});
    }

    t->createChartDisplayed();
    createMenuBar();
    createChartMenu();
    createWidgets();
    createEditMenu();
    createOptions();

    getChartSettings();
    updateChartSettings();
}

void MainWindow::on_apriJSON_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Apri grafico"), "", tr("Grafico JSON (*.json); Tutti i file (*.*)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {QMessageBox::information(this, tr("Impossibile aprire file"),file.errorString());
        throw file_exceptions("JSON");
    }
    Chart::importa(fileName);
    TableWidget::getDataFromFile();
    genericbox.setText("Importazione JSON avvenuta correttamente");
    genericbox.exec();
    if(genericbox.clickedButton()) go_back_c_clicked();
}

void MainWindow::on_esportaJSON_triggered(){
    if(c->isEmpty()){
        genericbox.setText("Il grafico è vuoto; inserire valori per salvare");
        genericbox.exec();
    }
    else{
        QString fileName = QFileDialog::getSaveFileName(this,tr("Salva grafico"), "", tr("Grafico JSON (*.json); Tutti i file (*.*)"));
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {QMessageBox::information(this, tr("Impossibile esportare file"),file.errorString());
            throw file_exceptions("JSON");
        }
        Chart::esporta(fileName);
        genericbox.setText("Esportazione JSON avvenuta correttamente");
        genericbox.exec();
        if(genericbox.clickedButton()){
            if(!c->isEmpty()) transitioning();
            go_back_c_clicked();
        }
    }
}

void MainWindow::on_apriXML_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Apri grafico"), "", tr("Grafico XML (*.xml); Tutti i file (*.*)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {QMessageBox::information(this, tr("Impossibile aprire file"),file.errorString());
        throw file_exceptions("XML");
    }
    Chart::importa(fileName);
    TableWidget::getDataFromFile();
    genericbox.setText("Apertura XML avvenuta correttamente");
    genericbox.exec();
    if(genericbox.clickedButton()) go_back_c_clicked();
}

void MainWindow::on_esportaXML_triggered(){
    if(c->isEmpty()){
        genericbox.setText("Il grafico è vuoto; inserire valori per salvare");
        genericbox.exec();
    }
    else{
        QString fileName = QFileDialog::getSaveFileName(this,tr("Salva grafico"), "", tr("Grafico XML (*.xml); Tutti i file (*.*)"));
        if (fileName.isEmpty()) return;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {QMessageBox::information(this, tr("Impossibile esportare file"),file.errorString());
        throw file_exceptions("XML");
        }
        Chart::esporta(fileName);
        genericbox.setText("Esportazione XML avvenuta correttamente");
        genericbox.exec();
        if(genericbox.clickedButton()){
            if(!c->isEmpty()) transitioning();
            go_back_c_clicked();
        }
    }
}

void MainWindow::on_add_row_clicked(){
    if(add_labels_axis->text() != nullptr){
        add_labels_axis->clear();
        QStringList attualeAsseX = t->getChartDisplayed()->getAxisX();
        attualeAsseX.append(add_labels_axis->text());
        t->getChartDisplayed()->setAxis(attualeAsseX);

        t->getMapper()->insertRow(t->getMapper()->rowCount()-1, 1, QModelIndex());
        t->setSeriesMapper(t->getMapper()->getOperationType());
        createWidgets();
    }
    else{
        genericbox.setText("Inserisci un valore nella riga per continuare");
        genericbox.exec();
    }
}

void MainWindow::on_delete_row_clicked(){
    t->getChartDisplayed()->rimuoviValoreAsseX(t->getChartDisplayed()->getAxisX().last());
    t->getMapper()->removeRow(t->getMapper()->rowCount()-1, 1, QModelIndex());
    t->setSeriesMapper(t->getMapper()->getOperationType());

    QHash<QString, QStringList> value;
    value["asseX"].append(c->getAxisX());

    c->setOption(c, value);

    if(t->getMapper()->rowCount()==0 || t->getMapper()->columnCount()==0){
        t->getChartDisplayed()->pulisciSerie();
    }
    createWidgets();
}

void MainWindow::on_add_column_clicked(){
    if(add_labels_series->text() != nullptr){
        if(dynamic_cast<CandleStick*>(t->getChartDisplayed())){
            if(!add_labels_series->text().toInt()){
                genericbox.setText("Inserisci un numero nella serie, non una stringa");
                genericbox.exec();
            }
        }
        add_labels_series->clear();
        t->getChartDisplayed()->insertLabel(add_labels_series->text());
        t->getMapper()->insertColumn(t->getMapper()->columnCount()-1, 1, QModelIndex());
        t->setSeriesMapper(t->getMapper()->getOperationType());
        createWidgets();
    }
    else{
        genericbox.setText("Inserisci un valore nella colonna per continuare");
        genericbox.exec();
    }
}

void MainWindow::on_remove_column_clicked(){
    if(dynamic_cast<line*>(t->getChartDisplayed())){
        t->getMapper()->removeColumn(t->getMapper()->columnCount()-1, 2, QModelIndex());
    }
    else{
        t->getMapper()->removeColumn(t->getMapper()->columnCount()-1, 1, QModelIndex());
    }
    t->setSeriesMapper(t->getMapper()->getOperationType());

    if(t->getMapper()->rowCount()==0 || t->getMapper()->columnCount()==0){
        t->getChartDisplayed()->pulisciSerie();
    }
    createWidgets();
}

