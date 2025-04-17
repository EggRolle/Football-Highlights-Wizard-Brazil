#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QListWidgetItem> // Or QStandardItemModel if using QTableView
#include "highlightWizard.h" // Include the backend logic

// Forward declarations for UI elements (if using Qt Designer promote)
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro required for classes with signals/slots

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots to connect to button clicks etc.
    void on_loadEventsButton_clicked();
    void on_findHighlightsButton_clicked();
    void on_loadVideoButton_clicked(); // Placeholder for video file selection

private:
    // --- UI Elements ---
    // If using Qt Designer:
    Ui::MainWindow *ui;
    // If creating UI manually:
    // QWidget *centralWidget;
    // QPushButton *loadEventsButton;
    // QPushButton *findHighlightsButton;
    // QPushButton *loadVideoButton;
    // QListWidget *eventsListWidget;
    // QListWidget *highlightsListWidget;
    // QLabel *eventFilePathLabel;
    // QLabel *videoFilePathLabel;
    // ... other layout managers etc.

    // --- Data Members ---
    HighlightWizard wizard;        // Instance of our backend logic class
    QString currentEventLogPath; // Store the path to the loaded log file
    QString currentVideoPath;    // Store the path to the selected video file

    // --- Helper Functions ---
    void setupUiManual();          // Call this in constructor if not using .ui file
    void displayEvents();          // Update the events list display
    void displayHighlights();      // Update the highlights list display
    void setStatusMessage(const QString& message, int timeout = 3000); // Show message in status bar
};
#endif // MAINWINDOW_H