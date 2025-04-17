#include "mainwindow.h"
#include "ui_mainwindow.h" // Include this if using Qt Designer's generated UI class
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug> // For printing debug messages

// If NOT using Qt Designer, you'll need to include necessary widgets here:
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar> // If adding status bar manually

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // If using Qt Designer
{
    // If using Qt Designer:
    ui->setupUi(this);
    // Optional: Set initial state
    ui->findHighlightsButton->setEnabled(false); // Disable until log is loaded
    ui->highlightsListWidget->addItem("Load an event log and click 'Find Highlights'.");
    ui->eventsListWidget->addItem("No event log loaded.");
    statusBar()->showMessage("Ready."); // Show initial status

    // --- If Creating UI Manually (Example - place this code instead of ui->setupUi(this)) ---
    /*
    setWindowTitle("Football Highlight Wizard");
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *fileLayout = new QHBoxLayout(); // Layout for file buttons/labels
    QHBoxLayout *listLayout = new QHBoxLayout(); // Layout for list views

    // Event Log Row
    loadEventsButton = new QPushButton("Load Event Log (.log)...");
    eventFilePathLabel = new QLabel("No log loaded.");
    eventFilePathLabel->setWordWrap(true);
    fileLayout->addWidget(loadEventsButton);
    fileLayout->addWidget(eventFilePathLabel, 1); // Stretch label

    // Video File Row (Placeholder)
    loadVideoButton = new QPushButton("Load Video File (.mp4)...");
    videoFilePathLabel = new QLabel("No video selected.");
    videoFilePathLabel->setWordWrap(true);
    QHBoxLayout *videoFileLayout = new QHBoxLayout();
    videoFileLayout->addWidget(loadVideoButton);
    videoFileLayout->addWidget(videoFilePathLabel, 1);


    // Lists Row
    eventsListWidget = new QListWidget();
    highlightsListWidget = new QListWidget();
    listLayout->addWidget(new QLabel("All Events:"));
    listLayout->addWidget(new QLabel("Highlights:"));
    QHBoxLayout* listWidgetsLayout = new QHBoxLayout();
    listWidgetsLayout->addWidget(eventsListWidget);
    listWidgetsLayout->addWidget(highlightsListWidget);


    // Find Highlights Button
    findHighlightsButton = new QPushButton("Find Highlights");
    findHighlightsButton->setEnabled(false); // Disable initially

    // Add layouts/widgets to main layout
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(videoFileLayout);
    mainLayout->addLayout(listLayout); // Add labels for lists
    mainLayout->addLayout(listWidgetsLayout); // Add the lists themselves
    mainLayout->addWidget(findHighlightsButton);


    // Set up status bar
    statusBar()->showMessage("Ready.");

    // Connect signals and slots MANUALLY
    connect(loadEventsButton, &QPushButton::clicked, this, &MainWindow::on_loadEventsButton_clicked);
    connect(findHighlightsButton, &QPushButton::clicked, this, &MainWindow::on_findHighlightsButton_clicked);
    connect(loadVideoButton, &QPushButton::clicked, this, &MainWindow::on_loadVideoButton_clicked);

    // Set initial text
    highlightsListWidget->addItem("Load an event log and click 'Find Highlights'.");
    eventsListWidget->addItem("No event log loaded.");
    */
    // ---------------- End Manual UI Setup Example -----------------
}

// Destructor
MainWindow::~MainWindow()
{
    // If using Qt Designer
    delete ui;
}

// --- Private Slots ---

void MainWindow::on_loadEventsButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open Event Log File",
        "", // Start directory (empty for default)
        "Log Files (*.log *.txt);;All Files (*)" // Filter
    );

    if (!filePath.isEmpty()) {
        currentEventLogPath = filePath;
        // Update UI label (assuming you have one named 'eventFilePathLabel')
        if (ui) ui->eventFilePathLabel->setText(QFileInfo(filePath).fileName()); // Show only filename
        // else eventFilePathLabel->setText(QFileInfo(filePath).fileName()); // If manual UI

        setStatusMessage("Loading events from: " + filePath);
        try {
            // Call the backend logic
            wizard.loadEventsFromFile(filePath.toStdString()); // Convert QString to std::string
            displayEvents(); // Update the list view
            ui->findHighlightsButton->setEnabled(true); // Enable the find button
            // Clear previous highlights if loading new file
            ui->highlightsListWidget->clear();
             ui->highlightsListWidget->addItem("Events loaded. Click 'Find Highlights'.");
            setStatusMessage("Events loaded successfully.", 5000);

        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error Loading Log", QString("Failed to load event log:\n%1").arg(e.what()));
            ui->findHighlightsButton->setEnabled(false); // Disable button on error
             ui->eventsListWidget->clear();
             ui->eventsListWidget->addItem("Failed to load events.");
            setStatusMessage("Error loading events.", 5000);
        }
    } else {
        setStatusMessage("Event log loading cancelled.");
    }
}

void MainWindow::on_findHighlightsButton_clicked()
{
    if (currentEventLogPath.isEmpty()) {
         QMessageBox::warning(this, "No Log File", "Please load an event log file first.");
        return;
    }

    setStatusMessage("Finding highlights...");
    try {
        // IMPORTANT: For long operations (like real video processing would be),
        // this should be run in a separate thread using QtConcurrent::run or QThread
        // to avoid freezing the GUI. For simple text parsing, it's likely okay here.
        wizard.findHighlights();
        displayHighlights(); // Update the highlight list view
        setStatusMessage("Highlight analysis complete.", 5000);

    } catch (const std::exception& e) {
        // Should not happen with current logic, but good practice
         QMessageBox::critical(this, "Error Finding Highlights", QString("An error occurred:\n%1").arg(e.what()));
         setStatusMessage("Error during highlight analysis.", 5000);
    }
}

void MainWindow::on_loadVideoButton_clicked()
{
     QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Video File",
        "", // Start directory
        "Video Files (*.mp4 *.avi *.mov);;All Files (*)" // Filter
    );

    if (!filePath.isEmpty()) {
        currentVideoPath = filePath;
        // Update UI label (assuming you have one named 'videoFilePathLabel')
        if (ui) ui->videoFilePathLabel->setText(QFileInfo(filePath).fileName());
        // else videoFilePathLabel->setText(QFileInfo(filePath).fileName()); // If manual UI
        setStatusMessage("Video file selected: " + QFileInfo(filePath).fileName(), 5000);
        QMessageBox::information(this, "Video Selected",
            QString("Video file selected:\n%1\n\n(Note: Video processing is not yet implemented)").arg(filePath));
        // ---- TODO: Future Implementation ----
        // - Store this path
        // - Potentially load video into a QMediaPlayer widget
        // - Enable functionality that uses this video (e.g., seeking based on highlights)
        // -----------------------------------

    } else {
         setStatusMessage("Video selection cancelled.");
    }
}


// --- Helper Functions ---

void MainWindow::displayEvents()
{
     // Choose ONE of these (ListWidget or TableView)
    // --- Using QListWidget ---
    if (ui) ui->eventsListWidget->clear();
    // else eventsListWidget->clear(); // If manual UI

    try {
         const auto& allEvents = wizard.getAllEvents(); // Assuming you add this getter to HighlightWizard
         if (allEvents.empty()){
              if (ui) ui->eventsListWidget->addItem("No events loaded or log was empty.");
              // else eventsListWidget->addItem("No events loaded or log was empty.");
              return;
         }

         for (const auto& event : allEvents) {
             QString itemText = QString("[%1'] %2: %3")
                                .arg(event.timestampMinutes)
                                .arg(QString::fromStdString(GameEvent::typeToString(event.type)))
                                .arg(QString::fromStdString(event.description));
             if (ui) ui->eventsListWidget->addItem(itemText);
             // else eventsListWidget->addItem(itemText); // If manual UI
         }
    } catch(const std::exception& e) {
         qWarning() << "Error retrieving events for display: " << e.what();
         if (ui) ui->eventsListWidget->addItem("Error displaying events.");
         // else eventsListWidget->addItem("Error displaying events.");
    }
     // --- OR Using QTableView (More complex setup required: QStandardItemModel) ---
    // ... involves creating a model, setting headers, populating rows/columns ...
}

void MainWindow::displayHighlights()
{
    // Choose ONE of these (ListWidget or TableView)
    // --- Using QListWidget ---
     if (ui) ui->highlightsListWidget->clear();
    // else highlightsListWidget->clear(); // If manual UI

    try {
        const auto& highlights = wizard.getHighlights();
        if (highlights.empty()) {
             if (ui) ui->highlightsListWidget->addItem("No highlights found.");
            // else highlightsListWidget->addItem("No highlights found."); // If manual UI
        } else {
            for (const auto& event : highlights) {
                QString itemText = QString("[%1'] %2: %3")
                                   .arg(event.timestampMinutes)
                                   .arg(QString::fromStdString(GameEvent::typeToString(event.type)))
                                   .arg(QString::fromStdString(event.description));
                if (ui) ui->highlightsListWidget->addItem(itemText);
                // else highlightsListWidget->addItem(itemText); // If manual UI
            }
        }
    } catch(const std::exception& e) {
         qWarning() << "Error retrieving highlights for display: " << e.what();
          if (ui) ui->highlightsListWidget->addItem("Error displaying highlights.");
         // else highlightsListWidget->addItem("Error displaying highlights.");
    }

    // --- OR Using QTableView ---
    // ... similar setup to displayEvents using a model ...
}

// Helper to show messages in the status bar
void MainWindow::setStatusMessage(const QString& message, int timeout) {
    if (statusBar()) { // Check if status bar exists
        statusBar()->showMessage(message, timeout);
        qDebug() << "Status:" << message; // Also log to debug output
    } else {
        qDebug() << "Status (no status bar):" << message;
    }
}