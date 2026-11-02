/**
 * @file EscalationRequest.h
 * @brief Declares the project interfaces and types.
 */

#ifndef ESCALATION_REQUEST_H
#define ESCALATION_REQUEST_H
#include <string>

/**
 * @brief Represents the EscalationRequest component of the workflow engine.
 */
class EscalationRequest {
private:
    std::string itemId_;
    int levelReached_ = 0;
public:
    /**
     * @brief Performs the EscalationRequest operation.
     * @param itemId Parameter used by this operation.
     * @return The result of the operation.
     */
    explicit EscalationRequest(const std::string& itemId);
    /**
     * @brief Performs the getItemId operation.
     * @return The result of the operation.
     */
    const std::string& getItemId() const;
    /**
     * @brief Performs the getLevelReached operation.
     * @return The result of the operation.
     */
    int getLevelReached() const;
    /**
     * @brief Performs the setLevelReached operation.
     * @param level Parameter used by this operation.
     */
    void setLevelReached(int level);
};

#endif // ESCALATION_REQUEST_H
