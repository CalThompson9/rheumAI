#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"
#include <QMessageBox>

AddPatientDialog::AddPatientDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPatientDialog) {
    ui->setupUi(this);
}

AddPatientDialog::~AddPatientDialog() {
    delete ui;
}

QString AddPatientDialog::getFirstName() const {
    return ui->firstNameInput->text().trimmed();  // 🔹 Trim spaces
}

QString AddPatientDialog::getLastName() const {
    return ui->lastNameInput->text().trimmed();
}

QString AddPatientDialog::getDateOfBirth() const {
    return ui->dobInput->text().trimmed();
}

void AddPatientDialog::accept() {
    // 🔹 Validate inputs
    if (getFirstName().isEmpty() || getLastName().isEmpty() || getDateOfBirth().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QDialog::accept();  // Close the dialog and return Accepted state
}
