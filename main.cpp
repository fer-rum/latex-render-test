#include <QProcess>

#include <iostream>
#include <QDir>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    // Do adapt!
    QDir miktexDir("D:/MikTeX_portable"); // <<< adapt this
    QDir sourceDir("D:/git/latex-render-test");
    QString texFileName = "example.tex";
    QString outputSubdir = "output";

    // Do not touch!
    QString latexCompilerName = "pdflatex.exe";

    Q_ASSERT(miktexDir.exists());
    Q_ASSERT(sourceDir.exists());

    QDir binarySubdir(miktexDir);
    binarySubdir.cd("texmfs/install/miktex/bin");
    Q_ASSERT(binarySubdir.exists());

    // create the output dir if neccessary
    QDir outputDir(sourceDir);
    if(!outputDir.exists(outputSubdir)){
        outputDir.mkdir(outputSubdir);
    }
    outputDir.cd(outputSubdir);

    // clean the output dir
    for(QString fileName : outputDir.entryList()){
        outputDir.remove(fileName);
    }

    QFile texSourceFile(sourceDir.filePath(texFileName));
    Q_ASSERT(texSourceFile.exists());

    // TODO programmatically maniplate source file

    Q_ASSERT(binarySubdir.exists(latexCompilerName));
    QString programPath = binarySubdir.filePath(latexCompilerName);

    QStringList arguments;
    arguments   << QString("-output-directory") // even on windows, but conform to no standard at all
                << outputDir.path()
                << QFileInfo(texSourceFile).filePath();


    std::cout << "Creating render process" << std::endl;
    QProcess* renderProcess = new QProcess();
    renderProcess->start(programPath, arguments);

    do {
        std::cout << renderProcess->readAllStandardOutput().toStdString();
        std::cerr << renderProcess->readAllStandardError().toStdString();
    } while( !renderProcess->waitForFinished(1000)); // every second as long as the process runs
    std::cout << "Render process finished" << std::endl;

    // TODO process needs watchdog, what if it crashes?
    // TODO check if this even produced output...

    return 0;
}
