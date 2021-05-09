
/*
	---------------VARIABLES THAT NEED DECLARATION-------------------
*/
#define VALIDATE_ERROR_LABEL validate_config_error
#define LINE_BUFFER linebuf
#define LINE_NUMBER lineno
#define IGNOREBUFFER discard
#define ERROR_LABEL parse_config_error

/*
	---------------PARSING OF CONFIG------------------------------
*/

#define IGNORE_LINE_START_WITH(P) { \
	if(starts_with(P, LINE_BUFFER)) { \
        LOG_MSG(DEBUG, ("Line %d skipped (comment)\n", LINE_NUMBER)); \
        continue; \
    } \
}

#define PARSE_INTEGER(NAME) { \
	if(starts_with(#NAME, LINE_BUFFER)) { \
	    sscanf(LINE_BUFFER, "%s %d", IGNOREBUFFER, &cm->NAME); \
	    LOG_MSG(INFO, (#NAME" size is %d bytes\n", \
	        (cm->NAME))); \
	    continue; \
	} \
}

#define PARSE_TIMEVAL(NAME) { \
	if(starts_with(#NAME, LINE_BUFFER)) { \
		int sec, usec; \
		sscanf(LINE_BUFFER, "%s %d %d", IGNOREBUFFER, &sec, &usec); \
		cm->NAME.tv_sec = sec; \
		cm->NAME.tv_usec = usec; \
		LOG_MSG(INFO, (#NAME" is %dsec, %dmicrosec\n", \
			(int)(cm->NAME.tv_sec), (int)(cm->NAME.tv_usec))); \
		continue; \
	} \
}

/*
	---------------VALIDATION OF CONFIG------------------------------
*/

#define VALIDATE_ADDRESS(A) { \
    if(inet_addr(A) == INADDR_NONE) { \
        printf("Error: invalid address: "#A" -> %s\n", A); \
        goto VALIDATE_ERROR_LABEL; \
    } \
}

#define VALIDATE_PORTNUMBER(P) { \
	if(P <= 1024) { \
        printf("Error: invalid port: "#P" should be above 1024\n"); \
        goto VALIDATE_ERROR_LABEL; \
    } \
}

#define VALIDATE_INT_NONZERO(I) { \
	if(I <= 0) { \
        printf("Error: invalid value for "#I": should be non-zero\n"); \
        goto VALIDATE_ERROR_LABEL; \
    } \
}

#define VALIDATE_INT_NONZERO_OR_DEFAULT(I, DEF) { \
	if(I <= 0) { \
		LOG_MSG(WARNING, ("WARNING: "#I" is NOT set\n")); \
		LOG_MSG(WARNING, ("Using default value ("#DEF")\n")); \
		I = DEF; \
    } \
}

#define VALIDATE_INT_COMPARISON(X, Y, OP) { \
	if(! X OP Y) { \
		printf("Error: improper values, "#X" (%d) "#OP" "#Y" (%d)\n", \
		(int)X, (int)Y); \
		goto VALIDATE_ERROR_LABEL; \
	} \
}

#define VALIDATE_TIMEVAL_NONZERO(TVP) { \
    if((TVP)->tv_sec*1000000 + (TVP)->tv_usec <= 0) { \
        printf("Error: interval "#TVP" cannot be zero\n"); \
        goto VALIDATE_ERROR_LABEL; \
    } \
}

#define VALIDATE_TIMEVAL_NONZERO_OR_DEFAULT(TVP, DS, DU) { \
    if((TVP)->tv_sec*1000000 + (TVP)->tv_usec <= 0) { \
		LOG_MSG(WARNING, ("WARNING: "#TVP" interval is NOT set\n")); \
		LOG_MSG(WARNING, ("Using default value ("#DS" "#DU")\n")); \
		(TVP)->tv_sec = DS; \
		(TVP)->tv_usec = DU; \
    } \
}

#define VALIDATE_TIMEVAL_GREATER_EQUAL_THAN(TVP1, TVP2) { \
	if((TVP2->tv_sec > TVP1->tv_sec) || \
	  (TVP2->tv_sec == TVP1->tv_sec && TVP2->tv_usec > TVP1->tv_usec)) { \
		printf("Error: "#TVP1" has to be >= than "#TVP2"!\n"); \
        goto VALIDATE_ERROR_LABEL; \
	} \
}

/*
	---------------GETTERS-------------------------------------------
*/

//This macros provide a quick way to define methods that return
// a field from the config_mngr struct (i.e. getters)
//The first is for pointers, the second for immediate values (or implicit pointers)
#define CONF_GETTER_P(N, T) \
    T lpconfig_get##_##N () { return &((lpconfig_get_manager_instance())->N); }
#define CONF_GETTER(N, T) \
    T lpconfig_get##_##N () { return ((lpconfig_get_manager_instance())->N); }
