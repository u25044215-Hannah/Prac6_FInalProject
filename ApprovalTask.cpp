#include "ApprovalTask.h"
#include "ApprovalHandler.h"
#include "ApprovalRequest.h"

void ApprovalTask::complete(WorkflowData result, Participant& actor) {
    if (approvals_) {
         ApprovalRequest request(getId(), result);
        if (!approvals_->handle(request)) {
            reject("Approval chain did not approve completion", actor);
            return;
        }
    }
    HumanTask::complete(result, actor);
}
