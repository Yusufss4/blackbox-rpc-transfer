/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _BLACKBOX_DATA_H_RPCGEN
#define _BLACKBOX_DATA_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

/// EDITED /// 
typedef enum {
    BLACKBOX_TRANSFER_START,
	BLACKBOX_TRANSFER_PROGRESS,
    BLACKBOX_TRANSFER_CANCEL
} blacbox_transfer_status_t;
/// EDITED /// 

struct blackbox_part_info {
	struct {
		u_int bin_packet_len;
		uint8_t *bin_packet_val;
	} bin_packet;
	u_int total_size;
	u_int packet_size;
	u_int bin_count;
	int transfer_status;
};
typedef struct blackbox_part_info blackbox_part_info;

struct blackbox_run_info {
	int input1;
	int input2;
};
typedef struct blackbox_run_info blackbox_run_info;

struct blackbox_response {
	int process_result;
	char *std_out;
	char *std_err;
};
typedef struct blackbox_response blackbox_response;

#define BLACKBOX_DATA 0x2fffffff
#define BLACKBOX_DATA_V1 1

#if defined(__STDC__) || defined(__cplusplus)
#define RUN_BLACKBOX 1
extern  blackbox_response * run_blackbox_1(blackbox_run_info *, CLIENT *);
extern  blackbox_response * run_blackbox_1_svc(blackbox_run_info *, struct svc_req *);
#define SEND_BLACKBOX 2
extern  int * send_blackbox_1(blackbox_part_info *, CLIENT *);
extern  int * send_blackbox_1_svc(blackbox_part_info *, struct svc_req *);
extern int blackbox_data_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define RUN_BLACKBOX 1
extern  blackbox_response * run_blackbox_1();
extern  blackbox_response * run_blackbox_1_svc();
#define SEND_BLACKBOX 2
extern  int * send_blackbox_1();
extern  int * send_blackbox_1_svc();
extern int blackbox_data_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_blackbox_part_info (XDR *, blackbox_part_info*);
extern  bool_t xdr_blackbox_run_info (XDR *, blackbox_run_info*);
extern  bool_t xdr_blackbox_response (XDR *, blackbox_response*);

#else /* K&R C */
extern bool_t xdr_blackbox_part_info ();
extern bool_t xdr_blackbox_run_info ();
extern bool_t xdr_blackbox_response ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_BLACKBOX_DATA_H_RPCGEN */
