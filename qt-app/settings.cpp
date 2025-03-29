/**
 * @file settings.cpp
 * @brief Definition of Settings class
 * @details The settings class handles user app configurations and settings menu construction.
 * @author Thomas Llamzon (tllamzon@uwo.ca)
 * @author Joelene Hales (jhales5@uwo.ca)
 * @date Mar. 16, 2025
 */

#include "settings.h"

/**
 * @brief Settings::instance - Defining default null value for static settings.
 */
Settings* Settings::instance = nullptr;

/**
 * @brief Set key file filename.
 */
QString Settings::keyFilename = "keyFile.txt";

/**
 * @name Settings (constructor)
 * @brief Constructor for Settings class
 * @details This constructor initializes the application's settings by reading
 * the key file to extract API keys and summary layout preference, and
 * setting API keys in their corresponding object. If a field is not found, it
 * is set to an empty string. The expected keys are the following:
 *      GEMINI_API_KEY: Google Gemini API key
 *      GOOGLE_AUDIO_API_KEY: Google Speech-to-Text API key
 *      OPENAI_AUDIO_API_KEY: OpenAI Whisper API key
 *      SUMMARY_LAYOUT_PREFERENCE: Summary layout preference. One of 
 *                                  {"Detailed Format", "Concise Format"}
 * @note The API keys are expected to be in the format "KEY_NAME:KEY_VALUE".
 * @param parent - MainWindow
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
Settings::Settings(QObject *p) : QObject(p), mainWindow(p)
{
    // Load default settings from key file
    QFile file(keyFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Settings key file" << keyFilename << " not found.";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        // Gemini API key
        if (line.startsWith("GEMINI_API_KEY:")) {
            file.close();
            llmKey = line.mid(QString("GEMINI_API_KEY:").length()).trimmed();
        }
        // Google audio API key
        else if (line.startsWith("GOOGLE_AUDIO_API_KEY:")) {
            file.close();
            googleSpeechApiKey = line.mid(QString("GOOGLE_AUDIO_API_KEY:").length()).trimmed();
        }
        // OpenAi audio API key
        else if (line.startsWith("OPENAI_AUDIO_API_KEY:")) {
            file.close();
            openAIAudioKey = line.mid(QString("OPENAI_AUDIO_API_KEY:").length()).trimmed();
        }
        // Summary layout preference
        else if (line.startsWith("SUMMARY_LAYOUT_PREFERENCE:")) {
            summaryLayoutPreference = line.mid(QString("SUMMARY_LAYOUT_PREFERENCE:").length()).trimmed();
        }
    }

    qDebug() << "Loaded settings:";
    qDebug() << "  GEMINI_API_KEY: " << llmKey;
    qDebug() << "  GOOGLE_AUDIO_API_KEY:" << googleSpeechApiKey;
    qDebug() << "  OPENAI_AUDIO_API_KEY:" << openAIAudioKey;
    qDebug() << "  SUMMARY_LAYOUT_PREFERENCE:" << summaryLayoutPreference;

    // Set API keys from keyFile
    LLMClient::getInstance()->setApiKey(llmKey);
    AudioHandler::getInstance()->setGoogleApiKey(googleSpeechApiKey);
    AudioHandler::getInstance()->setOpenAIApiKey(openAIAudioKey);
}

/**
 * @name Settings::getInstance
 * @brief Returns single static instance.
 * @details This function creates a singleton instance of the Settings class if it doesn't already exist.
 * @details It initializes the instance with the provided parameters and returns the instance.
 * @param parent - Main Window
 * @return Returns singleton settings object.
 * @author Joelene Hales
 * @author Thomas Llamzon
 */
Settings* Settings::getInstance(QObject *parent) {
    if (!instance) {
        instance = new Settings(parent);
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

    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Layout");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Layout");

    selectLayoutButton->setMenu(summaryLayoutOptions);
    selectLayoutButton->setText(summaryLayoutPreference);
    mainLayout->addLayout(summaryLayout);

    connect(optionDetailedLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Detailed Layout");
        selectLayoutButton->setText(summaryLayoutPreference);
    });
    connect(optionConciseLayout, &QAction::triggered, this, [=]() {
        setSummaryPreference("Concise Layout");
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
            okButtonClicked();
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
    QLineEdit *googleAudioKeyField = new QLineEdit(settingsWindow);
    googleAudioKeyField->setText(googleSpeechApiKey);
    QDialogButtonBox *audioButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    audioLayout->addWidget(audioLabel);
    audioLayout->addWidget(googleAudioKeyField);
    audioLayout->addWidget(audioButtonBox);
    mainLayout->addLayout(audioLayout);

    connect(audioButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!googleAudioKeyField->text().isEmpty()) {
            setGoogleSpeechApiKey(googleAudioKeyField->text());
            okButtonClicked();
        } else {
            qWarning() << "Google Audio key field cannot be empty.";
        }
    });
    connect(audioButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        googleAudioKeyField->clear();
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
            okButtonClicked();
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
 * @author Thomas Llamzon
 * @name readKey
 * @brief Retrieves a key from the keyfile
 * @returns Key value, or "" if key could not be found
 */
QString Settings::readKey(const QString& keyPrefix)
{
    QFile file(keyFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Settings key file" << keyFilename << " not found.";
        return "";
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith(keyPrefix)) {
            return line.mid(keyPrefix.length()).trimmed();
        }
    }

    return "";
}

/**
 * @name setLLMKey
 * @brief Sets LLM API key and modifies key storage file for continual use.
 * @details This function updates the LLM API key in the LLM client and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 */
void Settings::setLLMKey(QString newKey) {
    LLMClient::getInstance()->setApiKey(newKey);
    storeConfig("LLM", newKey);
}

/**
 * @name setGoogleSpeechApiKey
 * @brief Sets Google audio transcriber API key and modifies key storage file for continual use.
 * @details This function updates the audio API key in the AudioHandler client and stores it in a configuration file for future use.
 * @details This function is used to set the API key for Google audio transcriber.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setGoogleSpeechApiKey(QString newKey) {
    AudioHandler::getInstance()->setGoogleApiKey(newKey);
    storeConfig("AUDIO", newKey);
}

/**
 * @name setOpenAIAudioKey
 * @brief Sets OpenAI Whisper audio API key and stores it persistently.
 * @details This function updates the OpenAI audio API key in the AudioHandler client and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setOpenAIAudioKey(QString newKey) {
    AudioHandler::getInstance()->setOpenAIApiKey(openAIAudioKey);
    storeConfig("OPENAI_AUDIO", newKey);
}

/**
 * @name okButtonClicked
 * @brief Settings::setSummaryPreference
 * @details This function is called when the OK button is clicked in the settings dialog.
 * @param[in] pref - Summary Layout Preference (Detailed/Concise)
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setSummaryPreference(QString pref) {
    summaryLayoutPreference = pref;
    storeConfig("SUMM", pref);
}

/**
 * @name getLLMKey
 * @brief Get Gemini API key
 * @return Gemini API key
 * @author Joelene Hales
 */
QString Settings::getLLMKey() const {
    return llmKey;
}

/**
 * @name getGoogleSpeechApiKey
 * @brief Get Google Speech-to-Text API key
 * @return Google Speech-to-Text API key
 * @author Joelene Hales
 */
QString Settings::getGoogleSpeechApiKey() const {
    return googleSpeechApiKey;
}

/**
 * @name getOpenAIAudioKey
 * @brief Get Open AI Whisper API key
 * @return Open AI Whisper API key
 * @author Joelene Hales
 */
QString Settings::getOpenAIAudioKey() const {
    return openAIAudioKey;
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

    QFile file(keyFilename);
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
