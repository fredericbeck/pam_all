
#define PAM_SM_ACCOUNT
#define PAM_SM_AUTH
#define PAM_SM_PASSWORD
#define PAM_SM_SESSION

#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <stdarg.h>
#include <errno.h>
#include <pwd.h>

#define NAME	"pam_shamir.so"


void log_message(int level, char *msg, ...);

/* authentication management  */
PAM_EXTERN 
int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
        int retval, i;
	int debug_mod = 0; /* take LOG_DEBUG if debug option is specified in conf file */ 
	char **user = NULL;
	char *p = NULL;
	struct passwd *pwd = malloc(sizeof(struct passwd));

	if (pwd == NULL) { //call not_null
		log_message(LOG_CRIT, "malloc() %m");
		return PAM_SYSTEM_ERR;
	}

	#ifdef DEBUG
		debug_mod = LOG_DEBUG;
	#endif

	log_message(LOG_NOTICE, "the module was started");

	for (i=0; i<argc; i++) {
		if (strcmp(argv[i], "debug") == 0)
			debug_mod = LOG_DEBUG;
	}
	
	log_message(debug_mod, "debug: verbose mod is activated");

	if ((retval = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS) {
		log_message(LOG_ERR, "can not determine user name: %m");
		return retval;
	}

	log_message(debug_mod, "debug: user %s", user);

	if( (retval = pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &p, "%s", "Unix password : ")) != PAM_SUCCESS) {
		log_message(LOG_ERR, "can not determine the password");
		return retval;
	} 

	/*log_message(debug_mod, "debug: passwd %s", p);*/  /* You should never use this line (or give fake password) */

	free(pwd);
	return PAM_IGNORE;
}

PAM_EXTERN
int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv){
        return PAM_IGNORE;
}

/* account management */
PAM_EXTERN
int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv){
	return PAM_IGNORE;
}

/* session management */
PAM_EXTERN
int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv){
	return PAM_IGNORE;
}

PAM_EXTERN
int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv){
        return PAM_IGNORE;
}



void log_message(int level, char *msg, ...)
{
	va_list args;
	
	va_start(args, msg);
	openlog(NAME, LOG_PID, LOG_AUTHPRIV);
	
	if (level)
		vsyslog(level, msg, args);
	
	closelog();
	va_end(args);
}



