#include "WorkItemState.h"
#include "Task.h"

/* WorkItemState  
base:default =illegal */
void WorkItemState::reject_transition(const std::string& operation) const{
    throw IllegalTransitionException(name(), operation);
}

void WorkItemState::makeAvailable(Task&) { 
    reject_transition("makeAvailable");
}

void WorkItemState::assign(Task&, Participant&){ 
    reject_transition("assign");
}

void WorkItemState::acceptAssignment(Task&){
    reject_transition("acceptAssignment"); 
}

void WorkItemState::declineAssignment(Task&) { 
    reject_transition("declineAssignment"); 
}

void WorkItemState::start(Task&) {
    reject_transition("start");
}

void WorkItemState::complete(Task&, WorkflowData) { 
    reject_transition("complete"); 
}

void WorkItemState::fail(Task&)  { 
    reject_transition("fail"); 
}

void WorkItemState::reject(Task&){ 
    reject_transition("reject");
}

void WorkItemState::cancel(Task&) {
    reject_transition("cancel"); 
}

void WorkItemState::escalate(Task&){ 
    reject_transition("escalate"); 
}

void WorkItemState::hold(Task&){ 
    reject_transition("hold"); 
}

void WorkItemState::release(Task&){ 
    reject_transition("release"); 
}




//CreatedState
void CreatedState::makeAvailable(Task& t) { 
    t.setState(new AvailableState()); 
}




//AvailableState
void AvailableState::assign(Task& t, Participant& p) {
    t.setAssignee(p);
    t.setState(new AssignedState());
}

void AvailableState::hold(Task& t) {
    t.setState(new OnHoldState()); 
}

void AvailableState::cancel(Task& t) {
    t.setState(new CancelledState()); 
}





//AssignedState
void AssignedState::acceptAssignment(Task& t) {
    t.setState(new AssignedState()); //stays assigned until start()
}

void AssignedState::declineAssignment(Task& t) {
    t.clearAssignee();
    t.setState(new AvailableState()); //back into the pool for reassignment
}

void AssignedState::start(Task& t) {
    t.setState(new InProgressState()); 
}

void AssignedState::escalate(Task& t){
    t.setState(new EscalatedState()); 
}

void AssignedState::cancel(Task& t) {
    t.setState(new CancelledState()); 
}





//InProgressState
void InProgressState::complete(Task& t, WorkflowData r){
    t.setResult(r);
    t.setState(new CompletedState());
}
void InProgressState::fail(Task& t) {
    t.setState(new FailedState()); 
}

void InProgressState::reject(Task& t) {
    t.setState(new RejectedState());
}

void InProgressState::hold(Task& t){
    t.setState(new OnHoldState());
}

void InProgressState::escalate(Task& t) {
    t.setState(new EscalatedState());
}

void InProgressState::cancel(Task& t){
    t.setState(new CancelledState()); 
}




//OnHoldState
void OnHoldState::release(Task& t){
    t.setState(new AssignedState()); // resumes where it left off
} 
void OnHoldState::cancel(Task& t) {
    t.setState(new CancelledState()); 
}




//EscalatedState
void EscalatedState::assign(Task& t, Participant& p) {
    t.setAssignee(p);
    t.setState(new AssignedState()); // re-assigned to a new/same participant, de-escalated
}
void EscalatedState::fail(Task& t){
    t.setState(new FailedState()); 
}

void EscalatedState::cancel(Task& t){ 
    t.setState(new CancelledState()); 
}

