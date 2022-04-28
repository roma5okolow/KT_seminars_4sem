#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include "../log/log.h"

struct pam_conv my_conv =
{
		misc_conv,
		NULL,
};

//compile with -lpam -lpam_misc

int login_into_user(void)
{
	pam_handle_t *pam;
	int ret;

	printf("Enter username: ");
	char username[100];
	scanf("%s", username);

	ret = pam_start("my_ssh", username, &my_conv, &pam);

	if (ret != PAM_SUCCESS)
	{
		printf("Failed pam_start\n");
		log_error("Failed pam_start\n");
		return -1;
	}

	ret = pam_authenticate(pam, 0);

	if (ret != PAM_SUCCESS)
	{
		printf("Incorrect password!\n");
		log_error("Incorrect password!\n");
		return -1;
	}

	ret = pam_acct_mgmt(pam, 0);

	if (ret != PAM_SUCCESS)
	{
		printf("User account expired!\n");
		log_error("User account expired!\n");
		return -1;
	}

	if (pam_end(pam, ret) != PAM_SUCCESS)
	{
		printf("Unable to pam_end()\n");
		log_error("Unable to pam_end()\n");
		return -1;
	}

	printf("Login succesfull\n");
	log_info("Login succesfull\n");
	return 0;
}
