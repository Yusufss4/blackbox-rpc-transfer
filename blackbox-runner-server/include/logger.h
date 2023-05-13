#ifndef LOGGER_H
#define LOGGER_H

#define LOGGER_MAX_SERVER_NAME 100

/**
 * @brief Enum that defines the status of the log entry.
 *
 */
typedef enum log_status_t
{
    LOG_STATUS_SUCCESS = 0,
    LOG_STATUS_FAILURE = 1
} log_status_t;

typedef struct log_info_t
{
    log_status_t log_status;
    int input1;
    int input2;
    int output;

} log_info_t;

typedef struct logger_server_info_t
{
    char server_name[LOGGER_MAX_SERVER_NAME]; ///< The name of the server where logs will be sent.
    unsigned int server_port;

} logger_server_info_t;

/**
 * @brief Sends log message to the logger server
 * @param p_logger_server_info Pointer to logger server info
 * @param p_log_info Pointer to log info itself. The @c log_status_t will determine the how to log will be formatted in the server side.
 * @return Returns 0 on success and a non-zero value on failure.
 */
int logger_send_log_message(const logger_server_info_t *p_logger_server_info, const log_info_t *p_log_info);

/**
 * @brief Sets the server info for the logger
 *
 * @warning This function must be called before the @c logger_send_log_message.
 *          There can be only one server at a time. If you want to change the server,
 *          this function must be called again.
 *
 * @param p_server_name Pointer to server name
 * @param server_port Server port
 */
void logger_set_server_info(const char *p_server_name, unsigned int server_port);

/**
 * @brief Gets the server info for the logger
 * @param[out] p_logger_server_info Pointer to logger server info that will be filled.
 */
void logger_get_server_info(logger_server_info_t *p_logger_server_info);

#endif