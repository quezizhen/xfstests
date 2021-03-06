// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.
 * All Rights Reserved.
 */

#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <lib/dmport.h>
#include <lib/hsm.h>

#include <getopt.h>
#include <string.h>


/*---------------------------------------------------------------------------

Test program used to test the DMAPI function send_msg().  
The command line is:

	send_msg [-a] [-s sid] string

where string is the msgdata to be stored in the event.
sid is the session ID to use for the event.

----------------------------------------------------------------------------*/

#ifndef linux
extern	char	*sys_errlist[];
#endif
extern  int     optind;
extern  char    *optarg;


char	*Progname;

static void
usage(void)
{
	fprintf(stderr, "usage:\t%s [-a] [-s sid] string\n", Progname);
	exit(1);
}


int
main(
	int	argc, 
	char	**argv)
{
	dm_sessid_t	sid = DM_NO_SESSION;
	char		*string;
	char		*name;
	int		opt;
	dm_msgtype_t    msgtype = DM_MSGTYPE_SYNC;
	
	Progname = strrchr(argv[0], '/');
	if (Progname) {
		Progname++;
	} else {
		Progname = argv[0];
	}

	/* Crack and validate the command line options. */

	while ((opt = getopt(argc, argv, "as:")) != EOF) {
		switch (opt) {
		case 'a':
			msgtype = DM_MSGTYPE_ASYNC;
			break;
		case 's':
			sid = atol(optarg);
			break;
		case '?':
			usage();
		}
	}
	if (optind + 1 != argc)
		usage();
	string = argv[optind++];

	if (dm_init_service(&name) == -1)  {
		fprintf(stderr, "Can't initialize the DMAPI\n");
		exit(1);
	}
	
	if (sid == DM_NO_SESSION)
		find_test_session(&sid);
	
	if (dm_send_msg(sid, msgtype, strlen(string)+ 1, string)) {
		fprintf(stderr, "dm_send_msg failed, %s\n",
			strerror(errno));
		exit(1);
	}

	exit(0);
}

