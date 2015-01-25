#include <common.h>
#include <linux/string.h>
//#include <asm/arch/acs.h>
//#include <asm/arch/storage.h>
#include <errno.h>
#include <amlogic/efuse.h>
#include <malloc.h>
//#include <amlogic/secure_storage.h>
#include "key_manage.h"

/*
 * key_manage.c
 * this file support key read/write with key unite interface,and don't care about device for saving key
 * because there are many kinds of device and key, key should be configured in storage.c(board/amlogic/m6xxx/firmware/storage.c
 *  or customer/board/m6_xxxx/firmware/storage.c)
 *
 * the key_manage.c file should define below micro
 *     #define CONFIG_UNIFY_KEY_MANAGE
 * if the key unify interface can support all kinds of keys, depend on below micro
 * #define CONFIG_UNIFY_KEY_MANAGE
 *   #define CONFIG_OF
 *   #define CONFIG_EFUSE 1
 *
 *
 * */


#define KEY_NO_EXIST	0
#define KEY_BURNED		1

#define KEY_READ_PERMIT		10
#define KEY_READ_PROHIBIT	11

#define KEY_WRITE_MASK		(0x0f<<4)
#define KEY_WRITE_PERMIT	(10<<4)
#define KEY_WRITE_PROHIBIT	(11<<4)
//#define ACS_ADDR_ADDR	0xd9000200

/*function: key_unify_secure_boot_key
 * keyname:
 * keydata:
 * keylen:
 * return : 0: success, <0: fail, >0: exist
 * */
static int key_unify_secure_boot_key(char *keyname, unsigned char *keydata, int keylen)
{
	char str[128];
	int ret;
	char *securebootkey="secure_boot_set";
	if(strcmp(keyname, securebootkey) != 0){
		printf("keyname %s is eror\n",keyname);
		return -1;
	}
	keylen = keylen;
	memset(str,0,sizeof(str));
	sprintf(str,"efuse %s 0x%x",securebootkey,(int)keydata);
	ret = run_command(str, 0);
	return ret;
}

typedef int (* key_unify_dev_init)(char *buf,unsigned int len);
typedef int (* key_unify_dev_uninit)(void);

extern ssize_t uboot_key_put(char *device,char *key_name, char *key_data,int key_data_len,int ascii_flag);
extern ssize_t uboot_key_get(char *device,char *key_name, char *key_data,int key_data_len,int ascii_flag);
extern ssize_t uboot_key_query(char *device,char *key_name,unsigned int *keystate);

static int key_general_nand_init(char *buf,unsigned int len)
{
	return 0;
}
static int key_general_nand_write(char *keyname,unsigned char *keydata,unsigned int datalen,enum key_manager_df_e flag)
{
	int err = -EINVAL;

	return err;
}
static int key_general_nand_read(char *keyname,unsigned char *keydata,unsigned int datalen,unsigned int *reallen,enum key_manager_df_e flag)
{
	int err = -EINVAL;

	return err;
}
static int key_general_nand_query(char *keyname,unsigned int *keystate)
{
	int err = -EINVAL;
	return err;
}

static int key_securestorage_init(char *buf,unsigned int len)
{

	return 0;
}
static int key_securestorage_write(char *keyname,unsigned char *keydata,unsigned int datalen,enum key_manager_df_e flag)
{
	int err = -EINVAL;

	return err;
}
static int key_securestorage_read(char *keyname,unsigned char *keydata,unsigned int datalen,unsigned int *reallen,enum key_manager_df_e flag)
{
	int err = -EINVAL;

	return err;
}
static int key_securestorage_query(char *keyname,unsigned int *keystate)
{
	int err = -EINVAL;

	return err;
}
static int key_securestorage_uninit(void)
{

	return 0;
}

static int key_efuse_init(char *buf,unsigned int len)
{
	return 0;
}
static int key_efuse_write(char *keyname,unsigned char *keydata,unsigned int datalen,enum key_manager_df_e flag)
{
	return -EINVAL;
}
static int key_efuse_read(char *keyname,unsigned char *keydata,unsigned int datalen,unsigned int *reallen,enum key_manager_df_e flag)
{
	return -EINVAL;
}

static int _key_query_secure_boot_set(char* keyname, unsigned int * keystate)
{
    unsigned int pos = 0;
    unsigned int info_lis=0xffffffff;
    int nChkVal = 0, nChkAddr = 0;

    if(strcmp("secure_boot_set", keyname)){
        printf("Err, key name (%s) is not secure_boot_set\n", keyname);
        return -__LINE__;
    }

    //Check if bit7 && bit 6 are both 1
    //Attention: check this code to stay same with cmd[efuse secure_boot_set]
    efuse_read(&nChkVal,sizeof(nChkVal),(loff_t*)&nChkAddr);
    if(((nChkVal >> 7) & 1) && ((nChkVal >> 6) & 1))
    {
        *keystate = KEY_BURNED;
        return __LINE__;
    }

    *keystate = KEY_NO_EXIST;//key not burned
    return 0;
}

static int key_efuse_query(char *keyname,unsigned int *keystate)
{
	int err=-EINVAL;
	return err;
}

/*
 * function name: key_unify_init
 * buf : input
 * len  : > 0
 * return : >=0: ok, other: fail
 * */
int key_unify_init(char *buf,unsigned int len)
{
	int bakerr,err=-EINVAL;
	//enum key_manager_df_e key_dev[]={KEY_EFUSE_NORMAL,KEY_SECURE_STORAGE,KEY_GENERAL_NANDKEY};
	char *dev_node[]={"unkown","efuse","secure","general"};
	key_unify_dev_init dev_initfunc[]={NULL,key_efuse_init,key_securestorage_init,key_general_nand_init};
	int i,cnt;
	if(unifykey_dt_parse()){
		printf("%s:%d,unify key config table parse fail\n",__func__,__LINE__);
		return err;
	}
	bakerr = 0;
	cnt = sizeof(dev_initfunc)/sizeof(dev_initfunc[0]);
	for(i=0;i<cnt;i++){
		if(dev_initfunc[i]){
			err = dev_initfunc[i](buf,len);
			if(err < 0){
				printf("%s:%d,%s device ini fail\n",__func__,__LINE__,dev_node[i]);
				bakerr = err;
			}
		}
	}
	return bakerr;
}

/* funtion name: key_unify_write
 * keyname : key name is ascii string
 * keydata : key data buf
 * datalen : key buf len
 * return  0: ok, -0x1fe: no space, other fail
 * */
int key_unify_write(char *keyname,unsigned char *keydata,unsigned int datalen)
{
	int err=0;
	struct key_item_t *key_manage;
	enum key_manager_df_e key_df;
	key_manage = unifykey_find_item_by_name(keyname);
	if(key_manage == NULL){
		printf("%s:%d,%s key name is not exist\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(unifykey_item_verify_check(key_manage)){
		printf("%s:%d,%s key name is invalid\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(key_manage->permit & KEY_M_PERMIT_WRITE){
		err = -EINVAL;
		key_df = key_manage->df;
		switch(key_manage->dev){
			case KEY_M_EFUSE_NORMAL:
                {
                    if(!strcmp("secure_boot_set", keyname))
                    {
                        err = key_unify_secure_boot_key(keyname, keydata, datalen);
                    }
                    else
                    {
                        err = key_efuse_write(keyname,keydata,datalen,key_df);
                    }
                }
				break;
			case KEY_M_SECURE_STORAGE:
				err = key_securestorage_write(keyname,keydata,datalen,key_df);
				if(err == 0x1fe){
					err = -0x1fe;
				}
				break;
			case KEY_M_GENERAL_NANDKEY:
				err = key_general_nand_write(keyname,keydata,datalen,key_df);
				break;
			case KEY_M_UNKNOW_DEV:
			default:
				printf("%s:%d,%s key not know device\n",__func__,__LINE__,keyname);
				break;
		}
	}
	return err;
}

/*
 *function name: key_unify_read
 * keyname : key name is ascii string
 * keydata : key data buf
 * datalen : key buf len
 * reallen : key real len
 * return : <0 fail, >=0 ok
 * */
int key_unify_read(char *keyname,unsigned char *keydata,unsigned int datalen,unsigned int *reallen)
{
	int err=0;
	struct key_item_t *key_manage;
	enum key_manager_df_e key_df;
	key_manage = unifykey_find_item_by_name(keyname);
	if(key_manage == NULL){
		printf("%s:%d,%s key name is not exist\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(unifykey_item_verify_check(key_manage)){
		printf("%s:%d,%s key name is invalid\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(key_manage->permit & KEY_M_PERMIT_READ){
		err = -EINVAL;
		key_df = key_manage->df;
		switch(key_manage->dev){
			case KEY_M_EFUSE_NORMAL:
				err = key_efuse_read(keyname,keydata,datalen,reallen,key_df);
				break;
			case KEY_M_SECURE_STORAGE:
				err = key_securestorage_read(keyname,keydata,datalen,reallen,key_df);
				break;
			case KEY_M_GENERAL_NANDKEY:
				err = key_general_nand_read(keyname,keydata,datalen,reallen,key_df);
				break;
			case KEY_M_UNKNOW_DEV:
			default:
				printf("%s:%d,%s key not know device\n",__func__,__LINE__,keyname);
				break;
		}
	}
	return err;
}

/*
*    key_unify_query - query whether key was burned.
*    @keyname : key name will be queried.
*    @keystate: query state value, 0: key was NOT burned; 1: key was burned; others: reserved.
*     keypermit: read permit: bit0~bit3
*                write permit: bit4~bit7
*     if it return failed, keypermit is invalid; kerpermit is valid,when it return successful only
*    return: >=0: successful; others: failed.
*/
int key_unify_query(char *keyname,unsigned int *keystate,unsigned int *keypermit)
{
	int err=0;
	struct key_item_t *key_manage;
	enum key_manager_df_e key_df;
	key_manage = unifykey_find_item_by_name(keyname);
	if(key_manage == NULL){
		printf("%s:%d,%s key name is not exist\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(unifykey_item_verify_check(key_manage)){
		printf("%s:%d,%s key name is invalid\n",__func__,__LINE__,keyname);
		return -EINVAL;
	}
	if(key_manage->permit & KEY_M_PERMIT_READ){
		err = -EINVAL;
		key_df = key_manage->df;
		switch(key_manage->dev){
			case KEY_M_EFUSE_NORMAL:
                {
                    if(!strcmp("secure_boot_set", keyname)) {
                        err = _key_query_secure_boot_set(keyname, keystate);
                        *keypermit = KEY_READ_PROHIBIT;//not read!
                    }
                    else {
                        err = key_efuse_query(keyname,keystate);
                        *keypermit = KEY_READ_PERMIT;
                    }

                    if(err >= 0){
                        if(*keystate == KEY_BURNED){
                            *keypermit |= KEY_WRITE_PROHIBIT;
                        }
                        else if(*keystate == KEY_NO_EXIST){
                            *keypermit |= KEY_WRITE_PERMIT;
                        }
                    }
                }
                break;
			case KEY_M_SECURE_STORAGE:
				err = key_securestorage_query(keyname,keystate);
				*keypermit = KEY_READ_PROHIBIT;
				*keypermit |= KEY_WRITE_PERMIT;
				break;
			case KEY_M_GENERAL_NANDKEY:
				err = key_general_nand_query(keyname,keystate);
				*keypermit = KEY_READ_PERMIT;
				*keypermit |= KEY_WRITE_PERMIT;
				break;
			case KEY_M_UNKNOW_DEV:
				printf("%s:%d,%s key not know device\n",__func__,__LINE__,keyname);
			default:
				break;
		}
	}
	return err;
}

/* function name: key_unify_uninit
 * functiion : uninit
 * return : >=0 ok, <0 fail
 * */
int key_unify_uninit(void)
{
	int bakerr,err=-EINVAL;
	int i,cnt;
	key_unify_dev_uninit dev_uninit[]={key_securestorage_uninit};
	bakerr = 0;
	cnt = sizeof(dev_uninit)/sizeof(dev_uninit[0]);
	for(i=0;i<cnt;i++){
		if(dev_uninit[i]){
			err = dev_uninit[i]();
			if(err){
				bakerr = err;
			}
		}
	}
	return bakerr;
}




int do_keyunify(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int err;
	char *cmd,*name;
	unsigned int addr,len,reallen;
	unsigned int keystate,keypermit;

	if (argc < 2)
		goto usage;

	cmd = argv[1];
	if(!strcmp(cmd,"init")){
		if(argc > 3){
			addr = simple_strtoul(argv[2], NULL, 16);
			len  = simple_strtoul(argv[3], NULL, 16);
		}
		else{
			char initvalue[]={1,2,3,4};
			addr = (unsigned int)&initvalue[0];
			len  = sizeof(initvalue);
		}
		err = key_unify_init((char *)addr,len);
		return err;
	}
	if(!strcmp(cmd,"write")){
		if(argc < 5)
			goto usage;
		name = argv[2];
		addr = simple_strtoul(argv[3], NULL, 16);
		len  = simple_strtoul(argv[4], NULL, 16);
		err = key_unify_write(name,(unsigned char *)addr,len);
		if(err < 0){
			printf("%s:%d,%s key write fail\n",__func__,__LINE__,name);
		}
		return err;
	}
	if(!strcmp(cmd,"read")){
		if(argc < 6)
			goto usage;
		name = argv[2];
		addr = simple_strtoul(argv[3], NULL, 16);
		len  = simple_strtoul(argv[4], NULL, 16);
		reallen = simple_strtoul(argv[5], NULL, 16);
		err = key_unify_read(name,(unsigned char *)addr,len,(unsigned int*)reallen);
		if(err < 0){
			printf("%s:%d,%s key read fail\n",__func__,__LINE__,name);
		}
		return err;
	}
	if(!strcmp(cmd,"query")){
		if(argc < 5)
			goto usage;
		name = argv[2];
		keystate = simple_strtoul(argv[3], NULL, 16);
		keypermit  = simple_strtoul(argv[4], NULL, 16);
		err = key_unify_query(name,(unsigned int *)keystate,(unsigned int *)keypermit);
		if(err < 0){
			printf("%s:%d,%s key query fail\n",__func__,__LINE__,name);
		}
		if(err >=0){
			if(*(unsigned int*)keystate == KEY_BURNED){
				printf("%s key exist\n",name);
			}
			else{
				printf("%s key not exist\n",name);
			}
		}
		return err;
	}
    if(!strcmp(cmd,"secure")){
		addr = simple_strtoul(argv[3], NULL, 16);
		len  = simple_strtoul(argv[4], NULL, 16);
		err= key_unify_secure_boot_key(argv[2],(unsigned char *)addr,len);
		if(err < 0){
			printf("fail\n");
		}
		else if(err > 0){
			printf("exist\n");
		}
		else{
			printf("ok\n");
		}
		return err;
	}

	if(!strcmp(cmd,"uninit")){
		key_unify_uninit();
		return 0;
	}

usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(keyunify, CONFIG_SYS_MAXARGS, 1, do_keyunify,
	"key unify sub-system",
	"init [addr] [len]- show available NAND key name\n"
	"keyunify uninit - init key in device\n"
	"keyunify write keyname data-addr len  ---- wirte key data \n"
	"keyunify read keyname data-addr len reallen-addr the key data\n"
	"keyunify query keyname state-addr permit-addr"
);
