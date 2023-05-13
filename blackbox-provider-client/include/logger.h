#ifndef LOGGER_H
#define LOGGER_H

/**
 * @brief Append new log messages that starts with "SUCCESS:" to the file named
 *        @c p_file_name.  
 * 
 * @example The format for the log message is : SUCCESS:
 *                                              <input1><input2>: output 
 * 
 * @param[in] p_file_name File name for the log file name. 
 * @param[in] p_inputs Inputs that will be written to log message. 
 * @param[in] number_of_inputs Number of inputs.
 * @param[in] p_buffer Output message, check the example.  
 * @return int 0 if SUCCESS. 
 */
int write_success_result_to_log(const char *p_file_name, const int *p_inputs, int number_of_inputs, const char *p_buffer);

/**
 * @brief Append new log messages that starts with "FAIL:" to the file named
 *        @c p_file_name.  
 * 
 * @example The format for the log message is : FAIL:
 *                                              <input1><input2>: output 
 * 
 * @param[in] p_file_name File name for log name. 
 * @param[in] p_inputs Inputs that will be written to log message. 
 * @param[in] number_of_inputs Number of inputs.
 * @param[in] p_buffer Output message, check the example.  
 * @return int 0 if SUCCESS. 
 */
int write_failure_result_to_log(const char *p_file_name, const int *p_inputs, int number_of_inputs, const char *p_buffer);

#endif