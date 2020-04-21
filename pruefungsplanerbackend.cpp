#include "pruefungsplanerbackend.h"

#include <QQmlEngine>

void addTimeslots(Day* day){
    QList<Timeslot*> timeslots;

    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("1");
    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("2");
    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("3");
    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("4");
    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("5");
    timeslots.append(new Timeslot(day));
    timeslots.last()->setName("6");

    day->setTimeslots(timeslots);
}

void addDays(Week* week){
    QList<Day*> days;

    days.append(new Day(week));
    days.last()->setName("Montag");
    addTimeslots(days.last());
    days.append(new Day(week));
    days.last()->setName("Dienstag");
    addTimeslots(days.last());
    days.append(new Day(week));
    days.last()->setName("Mittwoch");
    addTimeslots(days.last());
    days.append(new Day(week));
    days.last()->setName("Donnerstag");
    addTimeslots(days.last());
    days.append(new Day(week));
    days.last()->setName("Freitag");
    addTimeslots(days.last());
    days.append(new Day(week));
    days.last()->setName("Samstag");
    addTimeslots(days.last());

    week->setDays(days);
}

Plan* createPlan(QObject* parent){
    Plan* m_plan;
    m_plan = new Plan(parent);
    m_plan->setName("Plan b");

    Module* module_a = new Module(m_plan);
    module_a->setName("Module a");
    module_a->setId("30.1234");
    module_a->setOrigin("FBI");
    module_a->setActive(true);
    Module* module_b = new Module(m_plan);
    module_b->setName("Module b");
    module_b->setId("30.1235");
    module_b->setOrigin("FBI");
    module_b->setActive(false);

    m_plan->modules.append(module_a);
    m_plan->modules.append(module_b);
    m_plan->modules.append(module_b);

    Group* constraint_a = new Group(m_plan);
    constraint_a->setName("Constraint a");
    Group* constraint_b = new Group(m_plan);
    constraint_b->setName("Constraint b");
    Group* constraint_c = new Group(m_plan);
    constraint_c->setName("Constraint c");
    Group* constraint_d = new Group(m_plan);
    constraint_d->setName("Constraint d");

    m_plan->constraints.append(constraint_a);
    m_plan->constraints.append(constraint_b);
    m_plan->constraints.append(constraint_c);
    m_plan->constraints.append(constraint_d);
    module_a->constraints.append(constraint_a);
    module_a->constraints.append(constraint_b);
    module_a->constraints.append(constraint_d);
    module_b->constraints.append(constraint_b);
    module_b->constraints.append(constraint_c);

    Group* group_a = new Group(m_plan);
    group_a->setName("group a");
    Group* group_b = new Group(m_plan);
    group_b->setName("group b");

    m_plan->groups.append(group_a);
    m_plan->groups.append(group_b);
    module_a->groups.append(group_a);
    module_a->groups.append(group_b);
    module_b->groups.append(group_a);

    Week* week_a = new Week(m_plan);
    week_a->setName("Week a");
    Week* week_b = new Week(m_plan);
    week_b->setName("Week b");
    Week* week_c = new Week(m_plan);
    week_c->setName("Week c");

    addDays(week_a);
    addDays(week_b);
    addDays(week_c);
    m_plan->weeks.append(week_a);
    m_plan->weeks.append(week_b);
    m_plan->weeks.append(week_c);

    QList<Group*> activeGroups;
    activeGroups.append(constraint_a);
    activeGroups.append(constraint_c);
    activeGroups.append(group_b);
    week_a->getDays().first()->getTimeslots().first()->setActiveGroups(activeGroups);
    week_a->getDays().last()->getTimeslots().last()->setActiveGroups(activeGroups);
    week_b->getDays().last()->getTimeslots().first()->setActiveGroups(activeGroups);
    week_c->getDays().first()->getTimeslots().last()->setActiveGroups(activeGroups);

    return m_plan;
}
PruefungsplanerBackend::PruefungsplanerBackend(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QQmlListProperty<Group>>("QQmlListProperty<Group>");

    Semester* semester_a = new Semester(this);
    Semester* semester_b = new Semester(this);
    semester_a->setName("Semester a");
    semester_b->setName("Semester b");

    QList<Plan*> plans_a;
    plans_a.append(createPlan(semester_a));
    plans_a.append(createPlan(semester_a));
    plans_a.first()->setName("plan a");
    plans_a.last()->setName("plan b");
    QList<Plan*> plans_b;
    plans_b.append(createPlan(semester_b));
    plans_b.append(createPlan(semester_b));
    plans_b.first()->setName("plan c");
    plans_b.last()->setName("plan d");

    semester_a->setPlans(plans_a);
    semester_b->setPlans(plans_b);

    semesters.append(semester_a);
    semesters.append(semester_b);

    activeSemester = semesters.first();
    m_plan = activeSemester->getPlans().first();
}

PruefungsplanerBackend* PruefungsplanerBackend::instance = nullptr;

PruefungsplanerBackend* PruefungsplanerBackend::getInstance()
{
    if (PruefungsplanerBackend::instance == nullptr)
        instance = new PruefungsplanerBackend;
    return instance;
}

QObject* PruefungsplanerBackend::getQmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return PruefungsplanerBackend::getInstance();
}

QString PruefungsplanerBackend::userName()
{
    return m_userName;
}

QAbstractListModel* PruefungsplanerBackend::testList(){
    std::cout << "A" << std::endl;
    QQuickView view;
    view.setSource(QUrl::fromLocalFile("../pruefungsplaner/dummydata/ModulesModel.qml"));
    view.show();
    QObject *object = (QObject*)view.rootObject();
    std::cout << "B" << std::endl;
    return ((QAbstractListModel*)object);
}

Plan* PruefungsplanerBackend::getActivePlan(){
    return m_plan;
}

void PruefungsplanerBackend::setActivePlan(Plan *plan)
{
    if (this->m_plan == plan)
        return;

    this->m_plan = plan;
    emit activePlanChanged();
}

Semester* PruefungsplanerBackend::getActiveSemester(){
    return activeSemester;
}

void PruefungsplanerBackend::setActiveSemester(Semester *semester)
{
    if (this->activeSemester == semester)
        return;

    this->activeSemester = semester;
    emit activeSemesterChanged();
}

void PruefungsplanerBackend::setSemesters(QList<Semester*> semesters)
{
    if (this->semesters == semesters)
        return;

    this->semesters = semesters;
    emit semestersChanged(this->semesters);
}

QList<Semester*> PruefungsplanerBackend::getSemesters() const
{
    return semesters;
}

void PruefungsplanerBackend::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}
