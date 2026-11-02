#include "FileExportAdapter.h"
#include "FileStorageService.h"
#include "WorkflowData.h"
#include <stdexcept>
FileExportAdapter::FileExportAdapter(FileStorageService* s) : service(s) {}
bool FileExportAdapter::isAvailable() const { return service != nullptr; }
WorkflowData FileExportAdapter::send(const WorkflowData& request) {
    if (!isAvailable()) throw std::runtime_error("File storage service not provided");
    const bool success = service->writeFile(request.get("path"), request.get("bytes"));
    WorkflowData response;
    response.set("success", success ? "true" : "false");
    return response;
}
