#include "settings.h"

// The getInstance method creates a single instance on first use.
// The instance is stored in a function-local static variable.
Settings* Settings::getInstance() {
    static Settings instance;
    return &instance;
}

// Private constructor: initializes the settings with default values.
Settings::Settings(QObject *parent)
    : QObject(parent), m_someValue(0)  // Initialize m_someValue to 0, for example.
{
}

// Getter for the example setting.
int Settings::someValue() const {
    return m_someValue;
}

// Setter for the example setting.
void Settings::setSomeValue(int value) {
    m_someValue = value;
}
