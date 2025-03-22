/**
 * @file settings.cpp
 * @brief Definition of Settings class
 * 
 * The settings class handles user app configurations and settings menu construction.
 * 
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
 */
Settings::Settings(QObject *p, LLMClient *llm, AudioHandler *audio)
    : QObject(p),
      mainWindow(p),
      llmClient(llm),
      audioHandlerClient(audio),
      llmKey(llm->apiKey),
      audioKey(audio->apiKey),
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

    // Connect dropdown menu option to change default summary format on
    // application load
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
 * @name setLLMKey
 * @brief Sets LLM API key and modifies key storage file for continual use.
 * @param[in] newKey: API key
 */
void Settings::setLLMKey(QString newKey) {
    llmClient->apiKey = newKey;
    storeConfig("LLM", newKey);
}

/**
 * @name setAudioKey
 * @brief Sets Google audio transcriber API key and modifies key storage file for continual use.
 * @param[in] newKey: API key
 */
void Settings::setAudioKey(QString newKey) {
    audioHandlerClient->apiKey = newKey;
    storeConfig("AUDIO", newKey);
}

/**
 * @name setOpenAIAudioKey
 * @brief Sets OpenAI Whisper audio API key and stores it persistently.
 * @param[in] newKey: API key
 */
void Settings::setOpenAIAudioKey(QString newKey) {
    audioHandlerClient->openAIApiKey = newKey;
    storeConfig("OPENAI_AUDIO", newKey);
}

/**
 * @brief Settings::setSummaryPreference
 * @param[in] pref - Summary Layout Preference (Detailed/Concise)
 */
void Settings::setSummaryPreference(QString pref) {
    summaryLayoutPreference = pref;
    storeConfig("SUMM", pref);
}


/**
 * @name getSummaryPreference
 * @brief Get the summary layout preference
 * @return Summary layout preference (Detailed/Concise)
 */
QString Settings::getSummaryPreference() const
{
    return summaryLayoutPreference;
}

/**
 * @name storeConfig
 * @brief Writes to hidden file storing user settings configurations.
 * @param[in] config - target setting to store
 * @param[in] value - value of user configuration
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

