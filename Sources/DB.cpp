#include "DB.h"

#include "SqlQuery.h"

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QSqlQuery>
#include <QTextCodec>
#include <QMessageBox>

const QString DbDriver = "QMYSQL"; //-? Or QMYSQL3 ?
const QString ConnectionName = "DBConnection";

DB * DB::mInstance = NULL;
//------------------------------------------------------------------------------
DB * DB::instance()
{
    if (!mInstance) {
        mInstance = new DB();
    }

    return mInstance;
}
//------------------------------------------------------------------------------
bool DB::openConnection(QString hostName, QString dbName,
                          QString userName, QString password, QString port)
{
    if (!QSqlDatabase::drivers().contains("QMYSQL")) {
        QMessageBox::critical(0, "Unable to load DB", "This application needs the QMYSQL driver");
    }
    else {
        //-QMessageBox::information(0, "OK", "Driver was found");
    }

    //-! Может не работает из-за того, что я еще connectionName передаю!?
    //-!Когда удаляю ConnectionName, то хоть что-то появляется

    db = QSqlDatabase::addDatabase(DbDriver);//-, ConnectionName); //-connectionName зачем это вообще надо мне?

    db.setHostName(hostName);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);
    db.setPort(port.toInt());

    if (!db.open()) {
        setLastError(db.lastError());

        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void DB::closeConnection()
{
    db.close();
    QSqlDatabase::database(ConnectionName).close(); //-?
    QSqlDatabase::removeDatabase(ConnectionName); //-?
}
//------------------------------------------------------------------------------
QString DB::lastErrorText() {
    return mLastErrorText;
}
//------------------------------------------------------------------------------
qint32 DB::lastErrorCode() {
    return mLastErrorCode;
}
//------------------------------------------------------------------------------
bool DB::executeScriptFromFile(QString hostName, QString dbName,
                                     QString userName, QString password,
                                     QString port, QString scriptPath) //- лучше передавать отдельно Path и fileName (там дальше выводится имя файла при ошибке)
{

//    if (!QSqlDatabase::drivers().contains("QMYSQL3")) {
//        QMessageBox::critical(0, "Unable to load database", "This application needs the QMYSQL driver");
//    }
//    else {
//        QMessageBox::information(0, "OK", "Driver was found");
//    }


    //- Здесь создадим локальное соединение с БД (т.е. временное, запишем инфу со скрипта, а затем закроем соединение)    (сама БД - НЕ обязательно локальная... так что лучше изменить название переменной). ЛУЧШЕ НЕ ИСПОЛЬЗОВАТЬ в качестве названия database.
    QSqlDatabase localDb = QSqlDatabase::addDatabase(DbDriver); //-?connection Name нужно или нет?

    /*-if(port == "")
        port = "3306"; // Default value for MySQL port. -*/ //- К одной базе одновременно через ОДИН порт подключаются или через разные?

    localDb.setHostName(hostName);
    localDb.setDatabaseName(dbName);
    localDb.setUserName(userName);
    localDb.setPassword(password);
    localDb.setPort(port.toInt());

    if (!localDb.open()) {
        setLastError(localDb.lastError());

        return false;
    }


    QFile creationScriptFile("template_script.sql"); //-?

    if(scriptPath.length() != 0) { //-scriptPath != ""
        creationScriptFile.setFileName(scriptPath);
    }

    if(!creationScriptFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, QObject::tr("Error"),
                QObject::tr("Error opening create DB script-file %1").arg(creationScriptFile.fileName()));
        return true;
    }
    QTextStream textStream(&creationScriptFile);
    textStream.setCodec(QTextCodec::codecForName("UTF-8"));

    QString szData(""); //-queryFromFile or queryFromScripFile
    QTextStream out(&creationScriptFile);
    QString szLine;
    do {
        szLine = out.readLine(); //-? Почему бы не воспользоваться readAll();
        szData += szLine + "\r\n";
    }
    while(!szLine.isNull());

    creationScriptFile.close();
    //-qDebug() << szData; //-
    QSqlQuery execQueryInScript; //- sqlQuery or query
    if(execQueryInScript.exec(szData)) {
        /*-QMessageBox::information(0,
                                 QObject::tr("Execution"),
                                 QObject::tr("Query from script-file successfully executed"));-*/
        localDb.close();
        return true;
    }
    else {
        setLastError(execQueryInScript.lastError());
        localDb.close();
        return false;
    }
}
//------------------------------------------------------------------------------
QString DB::getDiagnosisName(quint64 diagnosisId)
{
    SqlQuery diagnosisNameQuery;
    diagnosisNameQuery.prepare("SELECT Name FROM Diagnosis WHERE ID=?");
    diagnosisNameQuery.addBindValue(diagnosisId);
    diagnosisNameQuery.exec();
    diagnosisNameQuery.first();
    QString diagnosisName = diagnosisNameQuery.value(0).toString();

    return diagnosisName;
}
//------------------------------------------------------------------------------
QString DB::getPhaseName(quint64 phaseId)
{
    SqlQuery phaseNameQuery;
    phaseNameQuery.prepare("SELECT Name FROM Phase WHERE ID=?");
    phaseNameQuery.addBindValue(phaseId);
    phaseNameQuery.exec();
    phaseNameQuery.first();
    QString phaseName = phaseNameQuery.value(0).toString();

    return phaseName;
}
//------------------------------------------------------------------------------
QString DB::getParameterName(quint64 parameterId)
{
    SqlQuery parameterNameQuery;
    parameterNameQuery.prepare("SELECT Name FROM Parameter WHERE ID=?");
    parameterNameQuery.addBindValue(parameterId);
    parameterNameQuery.exec();
    parameterNameQuery.first();
    QString parameterName = parameterNameQuery.value(0).toString();

    return parameterName;
}
//------------------------------------------------------------------------------
QString DB::getResearchName(quint64 researchId)
{
    SqlQuery researchQuery;
    researchQuery.prepare("SELECT Name FROM Research WHERE ID=?");
    researchQuery.addBindValue(researchId);
    researchQuery.exec();
    researchQuery.first();
    QString researchName = researchQuery.value(0).toString();

    return researchName;
}
//------------------------------------------------------------------------------
QSet<quint64> DB::getParameterResearches(quint64 parameterId)
{
    // Получаем все исследования, с помощью которых можно получить значение
    // параметра:
    SqlQuery researchQuery;
    researchQuery.prepare("SELECT ResearchID FROM ParameterResearch WHERE ParameterID=?");
    researchQuery.addBindValue(parameterId);
    researchQuery.exec();

    QSet<quint64> researchIds;
    while (researchQuery.next()) {
        quint64 researchId = researchQuery.value(0).toULongLong();
        researchIds.insert(researchId);
    }

    return researchIds;
}
//------------------------------------------------------------------------------
bool DB::getDiagnosisId(QString diagnosisName, quint64 &diagnosisId)
{
    SqlQuery diagnosisQuery;
    diagnosisQuery.prepare("SELECT ID FROM Diagnosis WHERE Name=?");
    diagnosisQuery.addBindValue(diagnosisName);
    diagnosisQuery.exec();

    // Если нашли диагноз с таким названием:
    if (diagnosisQuery.size() > 0) {
        diagnosisQuery.first();
        diagnosisId = diagnosisQuery.value(0).toULongLong();

        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
QSet<QPair<quint64, QString> > DB::getDiagnosisParameters(quint64 diagnosisId)
{
    QSet<quint64> diagnosisPhases = getDiagnosisPhases(diagnosisId);

    QSet<QPair<quint64, QString> > diagnosisParameters;
    // Для каждой фазы диагноза находим список параметров:
    foreach (quint64 phaseId, diagnosisPhases) {
        QSet<quint64> parameterIds = getPhaseParameters(phaseId);

        // Добавляем все идентификаторы и названия параметров во множество:
        foreach (quint64 parameterId, parameterIds) {
            diagnosisParameters.insert(qMakePair(parameterId, getParameterName(parameterId)));
        }
    }

    return diagnosisParameters;
}
//------------------------------------------------------------------------------
QSet<quint64> DB::getDiagnosisPhases(quint64 diagnosisId)
{
    SqlQuery phaseQuery;
    phaseQuery.prepare("SELECT ID FROM Phase WHERE DiagnosisID=?");
    phaseQuery.addBindValue(diagnosisId);
    phaseQuery.exec();

    QSet<quint64> diagnosisPhases;
    while (phaseQuery.next()) {
        quint64 phaseId = phaseQuery.value(0).toULongLong();
        diagnosisPhases.insert(phaseId);
    }

    return diagnosisPhases;
}
//------------------------------------------------------------------------------
QSet<quint64> DB::getPhaseParameters(quint64 phaseId)
{
    SqlQuery parameterQuery;
    parameterQuery.prepare("SELECT ParameterID FROM PhaseParameter WHERE PhaseId=?");
    parameterQuery.addBindValue(phaseId);
    parameterQuery.exec();

    QSet<quint64> phaseParameters;
    while (parameterQuery.next()) {
        quint64 parameterId = parameterQuery.value(0).toULongLong();
        phaseParameters.insert(parameterId);
    }

    return phaseParameters;
}
//------------------------------------------------------------------------------
QList<QPair<quint64, QString> > DB::getAllDiagnosisIdsNames()
{
    SqlQuery diagnosisQuery;
    diagnosisQuery.exec("SELECT ID, Name FROM Diagnosis");

    QList<QPair<quint64, QString> > diagnosisIdsNames;
    while (diagnosisQuery.next()) {
        quint64 diagnosisId = diagnosisQuery.value(0).toULongLong();
        QString diagnosisName = diagnosisQuery.value(1).toString();

        diagnosisIdsNames.append(qMakePair(diagnosisId, diagnosisName));
    }

    return diagnosisIdsNames;
}
//------------------------------------------------------------------------------
QSet<QPair<QDateTime, Id> > DB::getPatientVerifications(Id patientId)
{
    SqlQuery verificationQuery;
    verificationQuery.prepare("SELECT DateTime, ID FROM Verification WHERE PatientID=?");
    verificationQuery.addBindValue(patientId);
    verificationQuery.exec();

    QSet<QPair<QDateTime, Id> > verifications;
    while (verificationQuery.next()) {
        QDateTime dateTime = verificationQuery.value(0).toDateTime();
        Id verificationId = verificationQuery.value(1).toULongLong();
        verifications.insert(qMakePair(dateTime, verificationId));
    }

    return verifications;
}
//------------------------------------------------------------------------------
QSet<QPair<QDateTime, Id> > DB::getPatientPrognoses(Id patientId)
{
    SqlQuery prognosisQuery;
    prognosisQuery.prepare("SELECT DateTime, ID FROM Prognosis WHERE PatientID=?");
    prognosisQuery.addBindValue(patientId);
    prognosisQuery.exec();

    QSet<QPair<QDateTime, Id> > prognoses;
    while (prognosisQuery.next()) {
        QDateTime dateTime = prognosisQuery.value(0).toDateTime();
        Id prognosisId = prognosisQuery.value(1).toULongLong();
        prognoses.insert(qMakePair(dateTime, prognosisId));
    }

    return prognoses;
}
//------------------------------------------------------------------------------
QString DB::getVerificationConclusion(Id expertiseId)
{
    SqlQuery query;
    query.prepare("SELECT Conclusion FROM Verification WHERE ID=?");
    query.addBindValue(expertiseId);
    query.exec();
    query.first();

    return query.value(0).toString();
}
//------------------------------------------------------------------------------
Id DB::getPhaseParameterId(Id phaseId, Id parameterId)
{
    SqlQuery query;
    query.prepare("SELECT ID FROM PhaseParameter WHERE PhaseID=? AND ParameterID=?");
    query.addBindValue(phaseId);
    query.addBindValue(parameterId);
    query.exec();
    query.first();
    return query.value(0).toULongLong();
}
//------------------------------------------------------------------------------
bool DB::addParameterResearches(Id parameterId, IdSet researchIds)
{
    foreach (Id researchId, researchIds) {
        addParameterResearch(parameterId, researchId);
    }
}
//------------------------------------------------------------------------------
bool DB::addParameterResearch(Id parameterId, Id researchId)
{
    SqlQuery query;
    query.prepare("INSERT INTO ParameterResearch (ID, ParameterID, ResearchID) VALUES (NULL, ?, ?)");
    query.addBindValue(parameterId);
    query.addBindValue(researchId);
    if (query.exec()) {
        return true;
    } else {
        return false;
    }
}
//------------------------------------------------------------------------------
bool DB::deleteParameterResearch(Id parameterId, Id researchId)
{
    SqlQuery query;
    query.prepare("DELETE FROM ParameterResearch WHERE ParameterID=? && ResearchID=?");
    query.addBindValue(parameterId);
    query.addBindValue(researchId);
    if (query.exec()) {
        return true;
    } else {
        return false;
    }
}
//------------------------------------------------------------------------------
bool DB::addPhaseParameter(Id phaseId, Id parameterId)
{
    SqlQuery query;
    query.prepare("INSERT INTO PhaseParameter (ID, PhaseID, ParameterID) VALUES (NULL, ?, ?)");
    query.addBindValue(phaseId);
    query.addBindValue(parameterId);
    if (query.exec()) {
        return true;
    } else {
        return false;
    }
}
//------------------------------------------------------------------------------
bool DB::deletePhaseParameter(Id phaseId, Id parameterId)
{
    SqlQuery query;
    query.prepare("DELETE FROM PhaseParameter WHERE PhaseID=? && ParameterID=?");
    query.addBindValue(phaseId);
    query.addBindValue(parameterId);
    if (query.exec()) {
        return true;
    } else {
        return false;
    }
}
//------------------------------------------------------------------------------
bool DB::addPhaseParameterRange(Id phaseId, Id parameterId,
        double rangeBegin, double rangeEnd, double beginProbability, double endProbability,
        QString description)
{
    Id phaseParameterId = getPhaseParameterId(phaseId, parameterId);
    SqlQuery query;
    query.prepare(QString("INSERT INTO PhaseParameterRange ")
            + QString("(ID, PhaseParameterID, RangeBegin, RangeEnd, BeginProbability, EndProbability, Description) ")
            + QString("VALUES (NULL, ?, ?, ?, ?, ?, ?)"));
    query.addBindValue(phaseParameterId);
    query.addBindValue(rangeBegin);
    query.addBindValue(rangeEnd);
    query.addBindValue(beginProbability);
    query.addBindValue(endProbability);
    query.addBindValue(description);
    if (query.exec()) {
        return true;
    } else {
        return false;
    }
}
//------------------------------------------------------------------------------
//%bool DB::addDiagnosis(const QString &name)
//{
//    SqlQuery query; //% query лучше вынести в поля класса и не объявлять каждый раз
//    query.prepare("INSERT INTO Diagnosis (ID, Name) VALUES (NULL, ?)");
//    query.addBindValue(name);
//    if (query.exec()) {
//        return true;
//    }

//    return false;
//}
//------------------------------------------------------------------------------
DB::DB()
{
}
//------------------------------------------------------------------------------
DB::~DB()
{
}
//------------------------------------------------------------------------------
void DB::setLastError(QSqlError error)
{
    mLastErrorText = error.text();
    mLastErrorCode = error.number();
}
//------------------------------------------------------------------------------
inline uint qHash(const QDateTime &key)
{
    return qHash(key.toString());
}
//------------------------------------------------------------------------------
