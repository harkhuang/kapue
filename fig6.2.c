#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


/**
 * 根据getpwent() ->获取进程工作环境变量  
**/


struct passwd *
getpwnam(const char *name)
{
    struct passwd  *ptr;

    setpwent();
    while ((ptr = getpwent()) != NULL)
        if (strcmp(name, ptr->pw_name) == 0)
        {
            printf("%s \n", ptr->pw_dir);
             printf("%ld \n", ptr->pw_expire);
              printf("%d \n", ptr->pw_gid);
               printf("%s \n", ptr->pw_shell);
             break;		/* found a match */
 
        }
           
    endpwent();
    return(ptr);	/* ptr is NULL if no match found */
}

int main()
{
    getpwnam("hk");
    return 0;
}