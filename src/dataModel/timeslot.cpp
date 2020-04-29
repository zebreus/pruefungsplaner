#include "timeslot.h"
#include "module.h"
class Module;

Timeslot::Timeslot(QObject *parent) : SerializableDataObject(parent)
{

}
QString Timeslot::name()
{
    return timeslotName;
}

void Timeslot::setName(const QString &name)
{
    if (name == timeslotName)
        return;

    timeslotName = name;
    emit nameChanged();
}

QList<Module*> Timeslot::getModules() const
{
    return this->modules;
}

QList<Group*> Timeslot::getActiveGroups() const
{
    return this->activeGroups;
}

void Timeslot::setModules(QList<Module*> modules)
{
    if (this->modules == modules)
        return;

    this->modules = modules;
    emit modulesChanged(this->modules);
}

void Timeslot::setActiveGroups(QList<Group*> activeGroups)
{
    if (this->activeGroups == activeGroups)
        return;

    this->activeGroups = activeGroups;
    emit activeGroupsChanged(this->activeGroups);
}

bool Timeslot::containsActiveGroup(Group* gp){
    return activeGroups.contains(gp);
}

void Timeslot::addActiveGroup(Group* gp){
    if (activeGroups.contains(gp)){
        return;
    }
    activeGroups.append(gp);
    emit activeGroupsChanged(this->activeGroups);
}

void Timeslot::removeActiveGroup(Group* gp){
    if (activeGroups.removeAll(gp) > 0){
        emit activeGroupsChanged(this->activeGroups);
    }
}


void Timeslot::fromJsonObject(const QJsonObject &content)
{
    simpleValuesFromJsonObject(content);

    Plan* activePlan = (Plan*) this->parent()->parent()->parent();

    QList<Group*> allGroups;
    allGroups.append(activePlan->getGroups());
    allGroups.append(activePlan->getConstraints());
    activeGroups = fromIdJsonArray<Group>(content.value("activeGroups"), allGroups);

    modules = fromIdJsonArray<Module>(content.value("modules"), activePlan->getModules());
}

QJsonObject Timeslot::toJsonObject() const
{
    QJsonObject jsonObject = recursiveToJsonObject();

    QJsonArray activeGroupArray;
    for(QJsonValueRef activeGroup: jsonObject.value("activeGroups").toArray()){
        int groupId = activeGroup.toObject().value("id").toInt();
        activeGroupArray.push_back(groupId);
    }
    jsonObject.insert("activeGroups", activeGroupArray);

    QJsonArray moduleArray;
    for(QJsonValueRef module: jsonObject.value("modules").toArray()){
        int moduleId = module.toObject().value("id").toInt();
        moduleArray.push_back(moduleId);
    }
    jsonObject.insert("modules", moduleArray);

    return jsonObject;
}
