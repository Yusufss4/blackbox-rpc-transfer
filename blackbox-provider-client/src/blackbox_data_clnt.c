/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "blackbox_data.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

blackbox_response *
run_blackbox_1(blackbox_run_info *argp, CLIENT *clnt)
{
	static blackbox_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, RUN_BLACKBOX,
		(xdrproc_t) xdr_blackbox_run_info, (caddr_t) argp,
		(xdrproc_t) xdr_blackbox_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

int *
send_blackbox_1(blackbox_part_info *argp, CLIENT *clnt)
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, SEND_BLACKBOX,
		(xdrproc_t) xdr_blackbox_part_info, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}