/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "logger_data.h"

bool_t
xdr_logger_info (XDR *xdrs, logger_info *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->log_string, ~0))
		 return FALSE;
	return TRUE;
}
