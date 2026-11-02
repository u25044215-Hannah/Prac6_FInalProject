#ifndef SMTPMAILSERVICE_H
#define SMTPMAILSERVICE_H

#include <string>

class SmtpMailService {
public:
    int sendMail(const std::string& to,
                 const std::string& subject,
                 const std::string& html);
};

#endif
