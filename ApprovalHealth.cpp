#include "ApprovalHandler.h"
#include <stdexcept>

void ApprovalHandler::setNext(ApprovalHandler* handler)
{
    if (handler == this) {
        throw std::invalid_argument("An approval handler cannot follow itself");
    }
    next_ = handler; // Non-owning: do not delete here.
}

bool ApprovalHandler::handle(ApprovalRequest& request)
{
    if (next_ != nullptr) {
        return next_->handle(request);
    }
    // Nobody in the chain was able to approve this request.
    return false;
}
