# Rheum AI
Rheum AI is a QT application that is designed to help rheumatologists and other healthcare professionals in summarizing
doctor-patient interaction. The program uses a combination of Open AI's Whisper model and Google's Speech API to transcribe a conversation. It then uses google's gemni 1.5 8b model to summarize the conversation. The program is designed to be used in a clinical setting, and it is intended to help healthcare professionals save time and improve patient care.

## Features
- Cross platform compatibility (Windows, macOS, Linux)
- Transcription of doctor-patient interaction
- Summarization of doctor-patient interaction
- Easy to use interface
- Fast and efficient
- Open source
- Built with QT and C++
- Uses Open AI's Whisper model and Google's Speech API for transcription
- Uses Google's Gemini 1.5 8b model for summarization

## Preview
![Preview](preview.png)

## Code Documentation
All classes and methods are documented using Doxygen-style comments.

To generate HTML documentation:

```
doxygen Doxyfile
```
This will create a `docs` folder with the generated documentation.

## License
This project is licensed under the Apache License 2.0.

## Folder Structure
The project contains the following folder structure:

```
deployment/
    linux/                  # Linux build with executable and dependencies
        rheumai
    mac/                    # macOS build with .app bundle
        RheumAI.app         # macOS application
    windows/                # Windows build with .exe and DLL dependencies
        rheumai.exe         # Executable for Windows
qt-app/
    main.cpp                # Main source file
    ... (other source files)
README                      # This file
```

## API Keys
- Google Speech API: You will need to set up your own Google Cloud account and enable the Speech-to-Text API. Follow the instructions [here](https://cloud.google.com/speech-to-text/docs/reference/rest/?apix=true) to get your API key.
- OpenAI Whisper: You will need to set up your own OpenAI account and get your API key. Follow the instructions [here](https://platform.openai.com/docs/api-reference/introduction) to get your API key. This is a **paid service so we will not** be providing the API key in the repository.
- Google Gemini: You will need to set up your own Google account and get your API key. Follow the instructions [here](https://console.cloud.google.com/apis/library/generativelanguage.googleapis.com?invt=AbtcGQ&project=gen-lang-client-0999730694) to get your API key.

## Provided API Keys
Check the submitted PDF file for our project. We will give the API keys for Google Speech and Google Gemini. The API key for OpenAI Whisper is a paid service and we will not be providing it.

## How to Run the Application

### Windows
1. Navigate to the windows/ folder.
2. Double-click on rheumai.exe to launch the application.
   - Ensure the dlls/ folder is in the same directory as rheumai.exe, as it contains required dependencies.

### macOS
1. Navigate to the mac/ folder.
2. Double-click on RheumAI.app to launch the application.
3. If you encounter a security warning about the app being from an unverified developer:
   - Open System Preferences > Security & Privacy.
   - Click Open Anyway to allow the app to run.

### Linux
1. Navigate to the linux/deployment/ folder.
2. Run the provided shell script:
   ```bash
   ./run.sh
   ```
   - This script sets up the required library paths and launches the application.
3. Make sure your system has a working display (X11 or Wayland) for the GUI.
4. Note: If Linux fails to run, you will have to open the project in QT creator, as shown below.

## How to Open the Project in Qt Creator
For those who wish to explore or modify the project, you can open the source code directly in Qt Creator.
### Steps to Open in Qt Creator
1. Install Qt Creator from https://www.qt.io/download.
2. Open Qt Creator and select File > Open File or Project.
3. Navigate to the qt-app/ folder and select the rheumai.pro file.
4. Select the appropriate build kit for your platform (e.g., MinGW for Windows, Clang for macOS, GCC for Linux).
5. Build and run the project directly from Qt Creator.

## Notes
- Cross-Platform Compatibility: This project has been tested on Windows and macOS ONLY. If you encounter any issues, please ensure the correct dependencies are present as described in the instructions above.
- Linux Display Requirement: The Linux build requires an active X11 or Wayland display to run the GUI.
- Dependencies: On Windows and Linux, the application depends on dynamic libraries (DLLs or .so files) that are included in the respective folders.

## Troubleshooting
- "Missing DLL" Error on Windows:
  - Ensure the dlls/ folder is in the same directory as mines.exe.

- "Could not connect to display" on Linux:
  - Make sure you have an active display server (X11 or Wayland).
  - If running on a headless server, set up a virtual display using Xvfb.

- macOS Security Warning:
  - See instructions under the macOS section above to allow the app to run.

## Acknowledgements
- OpenAI for the Whisper model.
- Google for the Speech-to-Text API and Gemini 1.5 8b model.
- Qt for the development framework.

## Contact
- **Andres Pedreros Castro (apedrero@uwo.ca)** for any questions related to audio recording and transcription.
- **Callum Thompson (cthom226@uwo.ca)** for any questions related to LLMs and summarization.
- **Joelene Hales (jhales5@uwo.ca)** for any questions related to design patterns used and the UML diagram.
- **Kalundi Serumaga (kserumag@uwo.ca)** for any questions related to file management and the folder structure.
- **Thomas Llamzon (tllamzon@uwo.ca)** for any questions related to the UI and functionality of the application.