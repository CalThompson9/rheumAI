/**
 * @file settings.cpp
 * @brief Definition of Settings class
 * @details The settings class handles user app configurations and settings menu construction.
 * @author Thomas Llamzon (tllamazon@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 16, 2025
 */

#include "settings.h"

/**
 * @brief Settings::instance - Defining default null value for static settings.
 */
Settings* Settings::instance = nullptr;

/**
 * @name Settings (constructor)
 * @brief Constructor for Settings class
 * @param parent - MainWindow
 * @param llm - LLM Client object
 * @param audio - Audio Handler object
 * @details This constructor initializes the settings object and loads the summary layout preference from a configuration file.
 * @author Joelene Hales
 * @author Thomas Llamzon
 */
Settings::Settings(QObject *p, LLMClient *llm, AudioHandler *audio)
    : QObject(p),
      mainWindow(p),
      llmClient(llm),
      audioHandlerClient(audio),
      llmKey(llm->apiKey),
      audioKey(audio->googleSpeechApiKey),
      openAIAudioKey(audio->openAIApiKey)
{
    // Load summary format from configuration file keyFile.txt
    summaryLayoutPreference = "";
    QFile file("keyFile.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "keyFile.txt not found.";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("SUMMARY_LAYOUT_PREFERENCE:")) {
            summaryLayoutPreference = line.mid(QString("SUMMARY_LAYOUT_PREFERENCE:").length()).trimmed();
        }
    }
}

/**
 * @name Settings::getInstance
 * @brief Returns single static instance.
 * @details This function creates a singleton instance of the Settings class if it doesn't already exist.
 * @details It initializes the instance with the provided parameters and returns the instance.
 * @param parent - Main Window
 * @param llm - LLM Client object
 * @param audio - Audio Handler object
 * @return Returns singleton settings object.
 * @author Joelene Hales
 * @author Thomas Llamzon
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
 * @details This function creates a dialog window for the settings menu, allowing users to modify their preferences.
 * @author Thomas Llamzon
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

    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Summary");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Summary");

    selectLayoutButton->setMenu(summaryLayoutOptions);
    selectLayoutButton->setText(summaryLayoutPreference);
    mainLayout->addLayout(summaryLayout);

    connect(optionDetailedLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Detailed Summary");
        selectLayoutButton->setText(summaryLayoutPreference);
    });
    connect(optionConciseLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Concise Summary");
        selectLayoutButton->setText(summaryLayoutPreference);
    });

    // ========== Connected Peripherals ==========
    QVBoxLayout *peripheralsLayout = new QVBoxLayout();
    QLabel *cpLabel = new QLabel("Connected Peripherals:", settingsWindow);
    peripheralsLayout->addWidget(cpLabel);

    QTextEdit *peripheralsField = new QTextEdit(settingsWindow);
    peripheralsField->setReadOnly(true);
    peripheralsField->setFixedHeight(60);
    peripheralsField->setStyleSheet("background-color: white; border: 1px solid gray;");

    const QList<QAudioDevice> audioDevices = QMediaDevices::audioInputs();
    peripheralsField->setText(
        !audioDevices.isEmpty()
            ? "Microphone Detected --> [" + audioDevices.first().description() + "]"
            : "No microphone detected."
        );

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
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    connect(llmButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        llmKeyField->clear();
    });

    // ========== Google Audio API Key ==========
    QHBoxLayout *audioLayout = new QHBoxLayout();
    QLabel *audioLabel = new QLabel("Google Transcriber API Key:", settingsWindow);
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
        } else {
            qWarning() << "Google Audio key field cannot be empty.";
        }
    });
    connect(audioButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        audioKeyField->clear();
    });

    // ========== OpenAI Audio API Key ==========
    QHBoxLayout *openaiLayout = new QHBoxLayout();
    QLabel *openaiLabel = new QLabel("OpenAI Whisper API Key:", settingsWindow);
    QLineEdit *openaiKeyField = new QLineEdit(settingsWindow);
    openaiKeyField->setText(openAIAudioKey);
    QDialogButtonBox *openaiButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    openaiLayout->addWidget(openaiLabel);
    openaiLayout->addWidget(openaiKeyField);
    openaiLayout->addWidget(openaiButtonBox);
    mainLayout->addLayout(openaiLayout);

    connect(openaiButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!openaiKeyField->text().isEmpty()) {
            setOpenAIAudioKey(openaiKeyField->text());
        } else {
            qWarning() << "OpenAI Audio key field cannot be empty.";
        }
    });
    connect(openaiButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        openaiKeyField->clear();
    });

    // ========== Close Settings Menu ==========
    QPushButton *closeButton = new QPushButton("Close", settingsWindow);
    mainLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, settingsWindow, &QDialog::close);

    // ========== Button Styling ==========
    QPushButton *llmOkButton = llmButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *llmCancelButton = llmButtonBox->button(QDialogButtonBox::Cancel);
    QPushButton *audioOkButton = audioButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *audioCancelButton= audioButtonBox->button(QDialogButtonBox::Cancel);
    QPushButton *openAIOkButton = openaiButtonBox->button(QDialogButtonBox::Ok);
    QPushButton *openAICancelButton = openaiButtonBox->button(QDialogButtonBox::Cancel);

    llmOkButton->setDefault(false);
    llmOkButton->setAutoDefault(false);
    audioOkButton->setDefault(false);
    audioOkButton->setAutoDefault(false);
    openAIOkButton->setDefault(false);
    openAIOkButton->setAutoDefault(false);

    llmOkButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);
    llmCancelButton->setStyleSheet(WindowBuilder::greyButtonStyle);
    audioOkButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);
    audioCancelButton->setStyleSheet(WindowBuilder::greyButtonStyle);
    openAIOkButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);
    openAICancelButton->setStyleSheet(WindowBuilder::greyButtonStyle);
    closeButton->setStyleSheet(WindowBuilder::settingsBlueButtonStyle);

    // ========================================
    settingsWindow->exec();
    delete settingsWindow;
}

/**
 * @name setLLMKey
 * @brief Sets LLM API key and modifies key storage file for continual use.
 * @details This function updates the LLM API key in the LLM client and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 */
void Settings::setLLMKey(QString newKey) {
    llmKey = newKey;
    llmClient->apiKey = newKey;
    storeConfig("LLM", newKey);
}

/**
 * @name setAudioKey
 * @brief Sets Google audio transcriber API key and modifies key storage file for continual use.
 * @details This function updates the audio API key in the AudioHandler client and stores it in a configuration file for future use.
 * @details This function is used to set the API key for Google audio transcriber.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 */
void Settings::setAudioKey(QString newKey) {
    audioKey = newKey;
    audioHandlerClient->googleSpeechApiKey = newKey;
    storeConfig("AUDIO", newKey);
}

/**
 * @name setOpenAIAudioKey
 * @brief Sets OpenAI Whisper audio API key and stores it persistently.
 * @details This function updates the OpenAI audio API key in the AudioHandler client and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 */
void Settings::setOpenAIAudioKey(QString newKey) {
    openAIAudioKey = newKey;
    audioHandlerClient->openAIApiKey = newKey;
    storeConfig("OPENAI_AUDIO", newKey);
}

/**
 * @name setSummaryPreference
 * @brief Stores summary preference code in the keyFile
 * @details This function is called when the OK button is clicked in the settings dialog.
 * @param[in] pref - Summary Layout Preference (Detailed/Concise)
 * @author Thomas Llamzon
 */
void Settings::setSummaryPreference(QString pref) {
    summaryLayoutPreference = pref;
    storeConfig("SUMM", pref);
}


/**
 * @name getSummaryPreference
 * @brief Get the summary layout preference
 * @details This function retrieves the summary layout preference from the settings.
 * @return Summary layout preference (Detailed/Concise)
 * @author Thomas Llamzon
 */
QString Settings::getSummaryPreference() const
{
    return summaryLayoutPreference;
}

/**
 * @name storeConfig
 * @brief Writes to hidden file storing user settings configurations.
 * @details This function stores user settings configurations in a hidden file for future use.
 * @param[in] config - target setting to store
 * @param[in] value - value of user configuration
 * @author Thomas Llamzon
 */
void Settings::storeConfig(const QString &config, const QString &value)
{
    QString prefix;
    if (config == "LLM") {
        prefix = "GEMINI_API_KEY:";
    } else if (config == "AUDIO") {
        prefix = "GOOGLE_AUDIO_API_KEY:";
    } else if (config == "OPENAI_AUDIO") {
        prefix = "OPENAI_AUDIO_API_KEY:";
    } else if (config == "SUMM") {
        prefix = "SUMMARY_LAYOUT_PREFERENCE:";
    } else {
        qWarning() << "Unknown keyClient:" << config;
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
            lines[i] = prefix + " " + value;
            foundLine = true;
            break;
        }
    }

    if (!foundLine) {
        lines << prefix + " " + value;
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
