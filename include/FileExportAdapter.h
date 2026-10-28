#ifndef FILEEXPORTADAPTER_H
#define FILEEXPORTADAPTER_H

#include "ExternalConnector.h"

class FileStorageService;

class FileExportAdapter : public ExternalConnector {
private:
    FileStorageService* service;

public:
    explicit FileExportAdapter(FileStorageService* service);

    WorkflowData send(const WorkflowData& request) override;
    bool isAvailable() const override;
};

#endif
