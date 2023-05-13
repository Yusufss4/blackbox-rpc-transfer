struct logger_info {
	string log_string<>;
};

program LOGGER_DATA {
	version LOGGER_DATA_V1 {
		int SAVE_LOG(logger_info) = 1;
	} = 1;
} = 0x2ffffff1;