#include "legato.h"
#include "interfaces.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef CHANGEROOTPASSWD
#define CHANGEROOTPASSWD "echo 'root:123456' | /usr/sbin/chpasswd"
#endif

void init_root();
char ChangeRootPasswd();

COMPONENT_INIT
{
    init_root();
}

void init_root()
{
    char ret = 0;
    char i = 0;

    for (i = 0; i < 3; i++)
    {
        ret = ChangeRootPasswd();
        if (0 != ret)
        {
            sleep(10);
        }
        else
        {
            return;
        }
    }

    LE_ERROR("root init failed, the passwd maybe not change.");
}

char ChangeRootPasswd()
{
        pid_t status;

	LE_INFO("shell command: %s\n", CHANGEROOTPASSWD);//JUST FOR DEBUG!!! please close it when you merge this app to your code.

	#if 1
        status = system(CHANGEROOTPASSWD) ;
	#else //debug
        status = system("echo \"echo \'root:123456\' | chpasswd\" > /home/root/123") ;
	status = system("chmod 777 /home/root/123");
        status = system("/home/root/123") ;
        //status = system("echo \'root:123456\' | chpasswd") ;
	#endif
        
	LE_INFO("WEXITSTATUS(status) = %d, WIFEXITED(stutus) = %d, status = %d\n",WEXITSTATUS(status), WIFEXITED(status), status);

        if (status == -1)	//system status
        {
                LE_ERROR("system error!\n") ;
		return -1;
        }

        if (WIFEXITED(status))	//exit status
        {
                //LE_INFO("exit staus = [%X]\n", WEXITSTATUS(status)) ;
        }
        else
        {
                LE_ERROR("exit error, exit staus = [%X]\n", WIFEXITED(status));
		return -1;
        }

        if (0 == WEXITSTATUS(status))
        {
                LE_INFO("run shell script successfully.\n");
        }
        else
        {
                LE_INFO("run shell command fail, exit code: %d\n", WEXITSTATUS(status)) ;
		return -1;
        }

	return 0;
}
