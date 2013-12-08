#include <QtGui/QApplication>
#include "MainWindow.h"

#include "Dialogs/DbConnectionDialog.h"

#include <QTranslator>
#include <QFile>

#include <QDebug> //%
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);


    // Загружаем русский перевод:
    QTranslator translator;
    qDebug() << "dirPath:" << QApplication::applicationDirPath();
    if (QFile::exists("LaDbEditor_ru.qm")) {
    } else {
        qWarning() << "No Ru-file!";
    }

    if (translator.load("LaDbEditor_ru.qm")) {
        application.installTranslator(&translator);
        qDebug() << "Ru-file was loaded.";
    } else {
        qDebug() << "Can not load Ru-file!";
    }


    DbConnectionDialog dialog;
    // Если успешно подключились к БД:
    if (dialog.exec()) {
        MainWindow mainWindow;
        mainWindow.show();
    
        return application.exec();
    } else {
        // Если пользователь нажал кнопку "Quit":
        return 0;
    }
}
//------------------------------------------------------------------------------
