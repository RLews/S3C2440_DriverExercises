/*
 * 
 *
 */

#include <common.h>
#include <command.h>
#include <def.h>
#include <nand.h>


extern char console_buffer[];
extern int readline (const char *const prompt);
extern char awaitkey(unsigned long delay, int* error_p);
extern void download_nkbin_to_flash(void);



void main_menu_usage(void)
{
    printf("\r\n#####    u-boot-1.1.6                           #####\r\n");
    printf("\r\n#####                 -- version 1.0.0.180520   #####\r\n");

}


void auto_load(void)
{
	printf("\r\nwait load update files...\r\n");
}

int do_auto (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    auto_load();

    run_command("reset", 0);/* completed. auto reset */
    return 0;
}

U_BOOT_CMD(
	AutoUpdate,	3,	0,	do_auto,
	"AutoUpdate - auto load SD Card or U disk update files, and download \n",
	" - aotu update "
);


