#include "settings.h"

/**
 * @author Thomas Llamzon
 */

/**
 * @brief Settings::instance - Defining default null value for static settings.
 */
Settings* Settings::instance = nullptr;

/**
 * @brief Constructor for Settings class; The settings class handles user app configurations and settings menu construction.
 * @param parent - MainWindow
 * @param llm - LLM Client object
 * @param audio - Audio Handler object
 */
Settings::Settings(QObject *p, LLMClient *llm, AudioHandler *audio)
    : QObject(p),
    mainWindow(p),
    llmClient(llm),
    audioHandlerClient(audio),
    llmKey(llm->apiKey),
    audioKey(audio->apiKey),
    summaryLayoutPreference("")
{
}

/**
 * @brief Settings::~Settings - Destructor
 */
Settings::~Settings() {
}

/**
 * @brief Returns single static instance.
 * @param parent - Main Window
 * @param llm - LLM Client object
 * @param audio - Audio Handler object
 * @return Returns singleton settings object.
 */
Settings* Settings::getInstance(QObject *parent, LLMClient *llm, AudioHandler *audio) {
    if (!instance) {
        instance = new Settings(parent, llm, audio);
    }
    return instance;
}

/**
 * @name Settings::showSettings
 * @brief Handles constructing the settings pop-up menu.
 */
void Settings::showSettings()
{
    QWidget *parent = (QWidget *)mainWindow;

    // ========== Layout ==========
    QDialog *settingsWindow = new QDialog(parent);
    settingsWindow->setWindowTitle("Settings");
    settingsWindow->setGeometry(0, 0, 800, 250);
    settingsWindow->adjustSize();

    QRect parentRect = parent->geometry();
    QSize dialogSize = settingsWindow->size();

    int x = parentRect.x() + ((parentRect.width()-dialogSize.width()) / 2);
    int y = parentRect.y() + ((parentRect.height()-dialogSize.height()) / 2);

    settingsWindow->move(x, y);

    QVBoxLayout *mainLayout = new QVBoxLayout(settingsWindow);

    // ========== Summary Layout Preference ==========
    QVBoxLayout *summaryLayout = new QVBoxLayout();

    QLabel *sLabel = new QLabel("Default Summary Layout Preference:", settingsWindow);
    summaryLayout->addWidget(sLabel);

    QPushButton *selectLayoutButton = new QPushButton(settingsWindow);
    QMenu *summaryLayoutOptions = new QMenu();
    summaryLayout->addWidget(selectLayoutButton);

    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");

    selectLayoutButton->setMenu(summaryLayoutOptions);
    selectLayoutButton->setText(summaryLayoutPreference);
    mainLayout->addLayout(summaryLayout);

    // Connect dropdown menu option to Summary Formatter
    connect(optionDetailedLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Detailed Layout");
        selectLayoutButton->setText(summaryLayoutPreference);

        // CODE HERE ?
    });
    connect(optionConciseLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Concise Layout");
        selectLayoutButton->setText(summaryLayoutPreference);

        // CODE HERE ?
    });

    // ========== Connected Peripherals ==========
    QVBoxLayout *peripheralsLayout = new QVBoxLayout();
    QLabel *cpLabel = new QLabel("Connected Peripherals:", settingsWindow);
    peripheralsLayout->addWidget(cpLabel);

    QTextEdit *peripheralsField = new QTextEdit(settingsWindow);
    peripheralsField->setReadOnly(true);
    peripheralsField->setFixedHeight(60); // Adjust height as needed
    peripheralsField->setStyleSheet("background-color: white; border: 1px solid gray;");

    // #################### TODO: Add microphone checking ####################
    if (true) {
        peripheralsField->setText("Microphone ✅");
    } else {
        peripheralsField->setText("Microphone ❌");
    }

    peripheralsLayout->addWidget(peripheralsField);
    mainLayout->addLayout(peripheralsLayout);

    // ========== LLM API Key ==========
    QHBoxLayout *llmLayout = new QHBoxLayout();
    QLabel *llmLabel = new QLabel("Summarizer API Key:", settingsWindow);
    QLineEdit *llmKeyField = new QLineEdit(settingsWindow);
    llmKeyField->setText(llmKey);
    QDialogButtonBox *llmButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    llmLayout->addWidget(llmLabel);
    llmLayout->addWidget(llmKeyField);
    llmLayout->addWidget(llmButtonBox);
    mainLayout->addLayout(llmLayout);

    connect(llmButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!llmKeyField->text().isEmpty()) {
            setLLMKey(llmKeyField->text());
            okButtonClicked();
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    connect(llmButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        llmKeyField->clear();
    });

    // ========== Audio Handler API Key ==========
    QHBoxLayout *audioLayout = new QHBoxLayout();
    QLabel *audioLabel = new QLabel("Transcriber API Key:", settingsWindow);
    QLineEdit *audioKeyField = new QLineEdit(settingsWindow);
    audioKeyField->setText(audioKey);
    QDialogButtonBox *audioButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    audioLayout->addWidget(audioLabel);
    audioLayout->addWidget(audioKeyField);
    audioLayout->addWidget(audioButtonBox);
    mainLayout->addLayout(audioLayout);

    connect(audioButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!audioKeyField->text().isEmpty()) {
            setAudioKey(audioKeyField->text());
            okButtonClicked();
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    connect(audioButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        audioKeyField->clear();
    });

    // ========== Close Settings Menu ==========
    QPushButton *closeButton = new QPushButton("Close", settingsWindow);
    mainLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, settingsWindow, &QDialog::close);

    QPushButton *llmOkButton = llmButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *llmCancelButton = llmButtonBox->button(QDialogButtonBox::Cancel);
    QPushButton *audioOkButton = audioButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *audioCancelButton= audioButtonBox->button(QDialogButtonBox::Cancel);

    // Disable default style
    llmOkButton->setDefault(false);
    llmOkButton->setAutoDefault(false);
    audioOkButton->setDefault(false);
    audioOkButton->setAutoDefault(false);

    llmOkButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);
    llmCancelButton->setStyleSheet(WindowBuilder::cancelStyle);
    audioOkButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);
    audioCancelButton->setStyleSheet(WindowBuilder::cancelStyle);
    closeButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);

    settingsWindow->exec();
    delete settingsWindow;
}

/**
 * @name Settings::setLLMKey
 * @brief Sets LLM API key and modifies key storage file for continual use.
 * @param key
 */
void Settings::setLLMKey(QString newKey) {
    llmClient->apiKey = newKey;
    writeToKeyFile("LLM", newKey);
}

/**
 * @name Settings::setAudioKey
 * @brief Sets audio transcriber API key and modifies key storage file for continual use.
 * @param key
 */
void Settings::setAudioKey(QString newKey) {
    audioHandlerClient->apiKey = newKey;
    writeToKeyFile("AUDIO", newKey);
}

/**
 * @brief Settings::setSummaryPreference
 * @param pref - Summary Layout Preference (Detailed/Concise)
 */
void Settings::setSummaryPreference(QString pref) {
    summaryLayoutPreference = pref;
    writeToKeyFile("SUMM", pref);
}

/**
 * @name writeToKeyFile
 * @brief Writes to hidden keyFile storing user settings configurations.
 * @param keyClient - Client to set API key for
 * @param key - User API key
 */
void Settings::writeToKeyFile(const QString &keyClient, const QString &key)
{
    QString prefix;
    if (keyClient == "LLM") {
        prefix = "GEMINI_API_KEY:";
    } else if (keyClient == "AUDIO") {
        prefix = "AUDIO_API_KEY:";
    } else if (keyClient == "SUMM") {
        prefix = "SUMMARY_LAYOUT_PREFERENCE:";
    } else {
        qWarning() << "Unknown keyClient:" << keyClient;
        return;
    }

    QFile file("keyFile.txt");
    QStringList lines;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines << in.readLine();
        }
        file.close();
    } else {
        if (file.error() != QFile::FileError::OpenError) {
            qWarning() << "Could not open file for reading:" << file.errorString();
            return;
        }
    }

    // Search for an existing line with the prefix and update it
    bool foundLine = false;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].startsWith(prefix)) {
            lines[i] = prefix + " " + key; // Replace everything after prefix
            foundLine = true;
            break;
        }
    }

    if (!foundLine) {
        lines << prefix + " " + key;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (const QString &line : lines) {
        out << line << "\n";
    }

    file.close();
}

