struct blackbox_part_info {
	uint8_t bin_packet<1000>;
	unsigned int total_size;
	unsigned int packet_size;
	unsigned int bin_count;
	int transfer_status;
};

struct blackbox_run_info {
	int input1;
	int input2;
};

struct blackbox_response {
	int process_result;
	string std_out<128>;
	string std_err<128>;
};

program BLACKBOX_DATA {
	version BLACKBOX_DATA_V1 {
		blackbox_response RUN_BLACKBOX(blackbox_run_info) = 1;
		int SEND_BLACKBOX(blackbox_part_info) = 2;
	} = 1;
} = 0x2fffffff;