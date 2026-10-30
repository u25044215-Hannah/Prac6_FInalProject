#include "ApprovalHandler.h"
#include "ApprovalRequest.h"
#include <stdexcept>

void ApprovalHandler::setNext(ApprovalHandler* handler) {
    if (handler == this) throw std::invalid_argument("Handler cannot follow itself");
    // Reject multi-node cycles as well, not just immediate self-links.
    for (ApprovalHandler* p = handler; p != nullptr; p = p->next_) {
        if (p == this) throw std::invalid_argument("Approval chain cycle");
    }
    next_ = handler;
}

bool ApprovalHandler::handle(ApprovalRequest& request) {
    return next_ ? next_->handle(request) : false;
}
