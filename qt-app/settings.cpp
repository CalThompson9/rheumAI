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

    // Read in keyFile to load in user configurations on app startup
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
 * @details This function creates a pop-up window for the settings menu, allowing users to modify their preferences.
 * @author Thomas Llamzon
 */
void Settings::showSettings()
{
    QWidget *parent = (QWidget *)mainWindow;

    // ========== Layout & Position ==========
    QDialog *settingsWindow = new QDialog(parent);
    settingsWindow->setWindowTitle("Settings");
    settingsWindow->setGeometry(0, 0, 800, 250);
    settingsWindow->adjustSize();

    // Move settings window to center of the main window
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

    // Construct default summary layout preference drop down menu
    QPushButton *selectLayoutButton = new QPushButton(settingsWindow);
    QMenu *summaryLayoutOptions = new QMenu();
    summaryLayout->addWidget(selectLayoutButton);

    QAction *optionDetailedLayout = summaryLayoutOptions->addAction("Detailed Summary");
    QAction *optionConciseLayout = summaryLayoutOptions->addAction("Concise Summary");

    selectLayoutButton->setMenu(summaryLayoutOptions);
    selectLayoutButton->setText(summaryLayoutPreference);
    mainLayout->addLayout(summaryLayout);

    // Connect selected user preference to storage in keyFile
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

    // Create text field for where the user's connected devices will be listed to them
    QTextEdit *peripheralsField = new QTextEdit(settingsWindow);
    peripheralsField->setReadOnly(true);
    peripheralsField->setFixedHeight(60);
    peripheralsField->setStyleSheet("background-color: white; border: 1px solid gray;");

    // Get conected devices through QAudioDevice library
    const QList<QAudioDevice> audioDevices = QMediaDevices::audioInputs();
    peripheralsField->setText(
        !audioDevices.isEmpty()
            ? "Microphone Detected --> [" + audioDevices.first().description() + "]"
            : "No microphone detected."
        );

    // Add to main settings window layout
    peripheralsLayout->addWidget(peripheralsField);
    mainLayout->addLayout(peripheralsLayout);

    // ========== Hide API Keys ==========
    // Declare API key QLineEdits so they are accessible in the checkbox lambda
    QLineEdit *llmKeyField = nullptr;
    QLineEdit *googleAudioKeyField = nullptr;
    QLineEdit *openaiKeyField = nullptr;

    // Create a checkbox that will toggle hiding/showing the API keys.
    QCheckBox *hideApiKeysCheckBox = new QCheckBox("Hide API Keys", settingsWindow);
    hideApiKeysCheckBox->setChecked(true);
    mainLayout->addWidget(hideApiKeysCheckBox);

    // ========== LLM API Key ==========
    QHBoxLayout *llmLayout = new QHBoxLayout();
    QLabel *llmLabel = new QLabel("Summarizer Key:", settingsWindow);
    llmKeyField = new QLineEdit(settingsWindow);
    llmKeyField->setEchoMode(QLineEdit::Password);
    llmKeyField->setText(llmKey);
    QDialogButtonBox *llmButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    // Configure layout of LLM API key configuration field
    llmLayout->addWidget(llmLabel);
    llmLayout->addWidget(llmKeyField);
    llmLayout->addWidget(llmButtonBox);
    mainLayout->addLayout(llmLayout);

    // Connect entered user API Key to storing in keyFile and updating relevant classes with this value.
    connect(llmButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!llmKeyField->text().isEmpty()) {
            setLLMKey(llmKeyField->text());
        } else {
            qWarning() << "This field cannot be empty.";
        }
    });
    connect(llmButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        llmKeyField->clear(); // Make cancel button clear the text field
    });

    // ========== Google Audio API Key ==========
    QHBoxLayout *audioLayout = new QHBoxLayout();
    QLabel *audioLabel = new QLabel("Transcriber Key (Google):", settingsWindow);
    googleAudioKeyField = new QLineEdit(settingsWindow);
    googleAudioKeyField->setEchoMode(QLineEdit::Password);
    googleAudioKeyField->setText(googleSpeechApiKey);
    QDialogButtonBox *audioButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    // Configure layout of Google Audio API key configuration field
    audioLayout->addWidget(audioLabel);
    audioLayout->addWidget(googleAudioKeyField);
    audioLayout->addWidget(audioButtonBox);
    mainLayout->addLayout(audioLayout);

    // Connect entered user API Key to storing in keyFile and updating relevant classes with this value.
    connect(audioButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!googleAudioKeyField->text().isEmpty()) {
            setGoogleSpeechApiKey(googleAudioKeyField->text());
        } else {
            qWarning() << "Google Audio key field cannot be empty.";
        }
    });
    connect(audioButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        googleAudioKeyField->clear(); // Make cancel button clear the text field
    });

    // ========== OpenAI Audio API Key ==========
    QHBoxLayout *openaiLayout = new QHBoxLayout();
    QLabel *openaiLabel = new QLabel("Transcriber Key (OpenAI):", settingsWindow);
    openaiKeyField = new QLineEdit(settingsWindow);
    openaiKeyField->setEchoMode(QLineEdit::Password);
    openaiKeyField->setText(openAIAudioKey);
    QDialogButtonBox *openaiButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, settingsWindow);

    // Configure layout of OpenAI API key configuration field
    openaiLayout->addWidget(openaiLabel);
    openaiLayout->addWidget(openaiKeyField);
    openaiLayout->addWidget(openaiButtonBox);
    mainLayout->addLayout(openaiLayout);

    // Connect entered user API Key to storing in keyFile and updating relevant classes with this value.
    connect(openaiButtonBox, &QDialogButtonBox::accepted, this, [=]() {
        if (!openaiKeyField->text().isEmpty()) {
            setOpenAIAudioKey(openaiKeyField->text());
        } else {
            qWarning() << "OpenAI Audio key field cannot be empty.";
        }
    });
    connect(openaiButtonBox, &QDialogButtonBox::rejected, this, [=]() {
        openaiKeyField->clear(); // Make cancel button clear the text field
    });

    // ========== Connect Hiding ==========
    connect(hideApiKeysCheckBox, &QCheckBox::toggled, this, [=](bool checked) {
        llmKeyField->setEchoMode(checked ? QLineEdit::Password : QLineEdit::Normal);
        googleAudioKeyField->setEchoMode(checked ? QLineEdit::Password : QLineEdit::Normal);
        openaiKeyField->setEchoMode(checked ? QLineEdit::Password : QLineEdit::Normal);
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

    // Disable default button styles to allow custom styles
    llmOkButton->setDefault(false);
    llmOkButton->setAutoDefault(false);
    audioOkButton->setDefault(false);
    audioOkButton->setAutoDefault(false);
    openAIOkButton->setDefault(false);
    openAIOkButton->setAutoDefault(false);

    // Assign custom button styles
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
 * @name readKey
 * @brief Retrieves a key value from the keyfile
 * @details This function reads the keyfile value for the provided key prefix.
 * It searches for the line that starts with the key prefix and returns the key value.
 * @note The key file is expected to be in the format "KEY_NAME:KEY_VALUE".
 * @returns Key value, or "" if key could not be found
 * @author Thomas Llamzon
 */
QString Settings::readKey(const QString& keyPrefix)
{
    // Open keyFile
    QFile file(keyFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Settings key file" << keyFilename << " not found.";
        return "";
    }

    // Find prefix in keyFile that indicates where the API key will be stored (similar to a dictionary)
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
 * @details This function updates the LLM API key in the LLM client and stores
 * it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setLLMKey(QString newKey) {
    llmKey = newKey;
    LLMClient::getInstance()->setApiKey(newKey);
    storeConfig("LLM", newKey);
}

/**
 * @name setGoogleSpeechApiKey
 * @brief Sets Google audio transcriber API key and modifies key storage file for continual use.
 * @details This function updates the Google Speech-to-Text API key in the
 * AudioHandler client and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setGoogleSpeechApiKey(QString newKey) {
    googleSpeechApiKey = newKey;
    AudioHandler::getInstance()->setGoogleApiKey(newKey);
    storeConfig("AUDIO", newKey);
}

/**
 * @name setOpenAIAudioKey
 * @brief Sets OpenAI Whisper audio API key and stores it persistently.
 * @details This function updates the OpenAI API key in the AudioHandler client
 * used and stores it in a configuration file for future use.
 * @param[in] newKey: API key
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setOpenAIAudioKey(QString newKey) {
    openAIAudioKey = newKey;
    AudioHandler::getInstance()->setOpenAIApiKey(openAIAudioKey);
    storeConfig("OPENAI_AUDIO", newKey);
}

/**
 * @name setSummaryPreference
 * @brief Stores summary preference code in the keyFile
 * @param[in] pref: Summary Layout Preference (Detailed/Concise)
 * @author Thomas Llamzon
 * @author Joelene Hales
 */
void Settings::setSummaryPreference(QString pref) {
    summaryLayoutPreference = pref;
    storeConfig("SUMM", pref);
}

/**
 * @name getLLMKey
 * @brief Get Google Gemini API key
 * @details This API key is used by LLMClient to summarize transcripts using the
 * Google Gemini API
 * @return Google Gemini API key
 * @author Joelene Hales
 */
QString Settings::getLLMKey() const {
    return llmKey;
}

/**
 * @name getGoogleSpeechApiKey
 * @brief Get Google Speech-to-Text API key
 * @details This API key is used by AudioHandler to transcribe speech using the
 * Google Speech-to-Text API
 * @return Google Speech-to-Text API key
 * @author Joelene Hales
 */
QString Settings::getGoogleSpeechApiKey() const {
    return googleSpeechApiKey;
}

/**
 * @name getOpenAIAudioKey
 * @brief Get Open AI Whisper API key
 * @details This API key is used by AudioHandler to transcribe speech using the
 * OpenAI Whisper API
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
 * @details This function stores user settings configurations in a hidden file
 * for future use.
 * @param[in] config: Target setting to store
 * @param[in] value: Value of user configuration
 * @author Thomas Llamzon
 */
void Settings::storeConfig(const QString &config, const QString &value)
{
    // Identify which API key the user wishes to store
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

    // Find and open keyFile
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

    // Create new line for new API key field (user's first time entering that API key)
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
