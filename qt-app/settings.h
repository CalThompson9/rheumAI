#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject {
    Q_OBJECT
public:
    // Returns the singleton instance of Settings
    static Settings* getInstance();

    // Getter and setter for an example setting
    int someValue() const;
    void setSomeValue(int value);

private:
    explicit Settings(QObject *parent = nullptr);
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    int m_someValue;
};

#endif // SETTINGS_H
