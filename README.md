# Qt Charts Creator & Renderer (Qt Charts CR)

A comprehensive Qt-based charting application that enables users to create, modify, and visualize various types of charts with an intuitive graphical interface and robust data management capabilities.

## 🎯 Project Overview

Qt Charts CR is an object-oriented application built with Qt 5.9+ that provides a complete charting solution for data visualization. The application leverages Qt Charts framework to offer four distinct chart types with extensive customization options and file I/O capabilities.

## ✨ Features

### Chart Types
- **Line Charts**: Multi-series line visualization with customizable axes
- **Bar Charts**: Vertical bar charts with category-based data representation
- **Pie Charts**: Circular data visualization with optional donut mode
- **Candlestick Charts**: Financial data visualization with OHLC (Open, High, Low, Close) values

### Core Functionality
- **Interactive Data Editing**: Real-time table-based data modification
- **Dynamic Chart Options**: Theme selection, animations, antialiasing, and legend control
- **Axis Customization**: Configurable X/Y axes with custom labels and ranges
- **File I/O Support**: Import/export charts in JSON and XML formats
- **Exception Handling**: Comprehensive error management for file operations and data validation
- **Cross-Platform**: Compatible with Windows and Linux environments

## 🏗️ Architecture

### Object-Oriented Design
The application follows a hierarchical class structure:

- **Abstract Base Class**: `Chart` - Defines common interface for all chart types
- **Concrete Implementations**: `Line`, `Bar`, `Pie`, `CandleStick` - Specific chart implementations
- **Model-View Architecture**: Separation of data logic and presentation
- **Exception Hierarchy**: Custom exception classes for different error types

### Key Components
- **Chart Classes**: Core logic for data management and chart rendering
- **Mapper**: Qt Model/View framework implementation for table editing
- **TableWidget**: View component for chart and table display
- **MainWindow**: Main GUI controller with dynamic widget management
- **Exception System**: Comprehensive error handling for runtime safety

## 📋 Requirements

- **Qt Version**: 5.9.5 or higher
- **Compiler**: 
  - Windows: MinGW 5.3.0+ or MSVC
  - Linux: GCC with multilib support
- **Dependencies**: Qt Charts module, Qt XML module

## 🚀 Installation & Build

### Windows
```bash
# Using Qt Creator
1. Open Qt_Charts_CR.pro in Qt Creator
2. Configure kit (MinGW recommended)
3. Build and run

# Command line
qmake Qt_Charts_CR.pro
make
```

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt install qt5-default qtcharts5-dev libqt5xml5-dev
sudo apt install gcc-multilib g++-multilib

# Build
qmake Qt_Charts_CR.pro
make
```

## 📖 Usage

### Getting Started
1. **Launch Application**: Run the executable to open the main interface
2. **Select Chart Type**: Choose from Line, Bar, Pie, or Candlestick charts
3. **Initial Setup**: Enter initial data values and axis labels
4. **Data Entry**: Use the interactive table to modify chart data
5. **Customization**: Adjust themes, colors, and display options
6. **Save/Load**: Export charts to JSON/XML or import existing chart files

### Data Management
- **Add/Remove Series**: Use the editing controls to modify data series
- **Real-time Updates**: Charts update automatically as you edit table data
- **Axis Configuration**: Customize X/Y axis labels and ranges
- **Color Mapping**: Visual feedback with color-coded table cells

### File Operations
- **Supported Formats**: JSON and XML for cross-compatibility
- **Import**: Load existing chart configurations and data
- **Export**: Save charts with all settings and data preserved

## 📁 Project Structure

```
Qt-Charts_CR/
├── Qt_Charts_CR/           # Main source directory
│   ├── *.cpp, *.h          # Source and header files
│   ├── mainwindow.ui       # UI layout file
│   ├── Qt_Charts_CR.pro    # Qt project file
│   ├── risorse.qrc         # Resource file
│   └── icons/              # Application icons
├── Esempi_file_import-export/ # Sample files
│   ├── *.json              # JSON examples
│   └── *.xml               # XML examples
├── feedback.txt            # Project evaluation feedback
└── Relazione.pdf          # Detailed technical documentation
```

## 🛠️ Advanced Features

### Exception Handling
- **File Exceptions**: Robust handling of corrupted or invalid files
- **Logic Exceptions**: Data validation and consistency checks
- **Graphics Exceptions**: UI error management and recovery
- **Segmentation Fault Management**: Custom crash reporting and logging

### Polymorphism Implementation
- **Virtual Methods**: `insert()`, `setAxis()`, `finaliza()`, `reloadGrafico()`
- **Template Support**: Generic data structures for extensibility
- **Dynamic Type Management**: Runtime chart type determination
- **Memory Management**: Automatic cleanup and resource management

## 🐛 Known Issues

### Linux-Specific Warnings
- **XCB Connection Warning**: Display-related warning on Ubuntu VM (non-critical)
- **GTK Dialog Warning**: Native file dialog warning (functionality preserved)

## 👥 Contributors

- **Andrea Crocco** (Matr. 1226135)
- **Gabriel Rovesti** (Matr. 2009088)

## 📄 License

This project was developed as part of an Object-Oriented Programming course at the University of Padova (2021/2022).

## 📚 Documentation

For detailed technical specifications, architecture documentation, and implementation details, refer to `Relazione.pdf` included in the repository.

---

**Note**: This application demonstrates advanced Qt programming concepts including the Model/View framework, custom widgets, XML/JSON processing, and comprehensive object-oriented design patterns.