#ifndef __CONFIG_M8_K200_V1_H__
#define __CONFIG_M8_K200_V1_H__

#define CONFIG_M8				1
#define CONFIG_AML_MESON_8      1
#define CONFIG_MACH_MESON8_K200_V1  // generate M8 K200 machid number

//UART Sectoion
#define CONFIG_CONS_INDEX   2



//#define  CONFIG_AML_GATE_INIT	1
//support "boot,bootd"
//#define CONFIG_CMD_BOOTD 1

//Enable HDMI Tx
//#define CONFIG_VIDEO_AMLTVOUT 1
//Enable LCD output
//#define CONFIG_VIDEO_AMLLCD
#define LCD_BPP LCD_COLOR16


#define CONFIG_DDR_SIZE_IND_ADDR 0xD9000000	//pass memory size, spl->uboot



#define CONFIG_CMD_PWM  1
//#define CONFIG_CMD_IMGREAD_FOR_SECU_BOOT_V2 1  //open this macro if need read encrypted kernel/dtb with whole part size

//Enable storage devices
//#define CONFIG_CMD_NAND  1
#define CONFIG_VIDEO_AML 1
#define CONFIG_CMD_BMP 1
#define CONFIG_VIDEO_AMLTVOUT 1
#define CONFIG_AML_HDMI_TX  1
#define CONFIG_OSD_SCALE_ENABLE 1

//Enable storage devices
#define CONFIG_CMD_SF    1

#if defined(CONFIG_CMD_SF)
	#define SPI_WRITE_PROTECT  1
	#define CONFIG_CMD_MEMORY  1
#endif /*CONFIG_CMD_SF*/

//Amlogic SARADC support
#define CONFIG_SARADC 1
#define CONFIG_CMD_SARADC
//#define CONFIG_EFUSE 1
//#define CONFIG_MACHID_CHECK 1

//#define CONFIG_IR_REMOTE 1
#define CONFIG_L2_OFF	 1

#define CONFIG_CMD_NET   1
#if defined(CONFIG_CMD_NET)
	#define CONFIG_AML_ETHERNET 1
	#define CONFIG_NET_MULTI 1
	#define CONFIG_CMD_PING 1
	#define CONFIG_CMD_DHCP 1
	#define CONFIG_CMD_RARP 1
	//#define CONFIG_NET_RGMII
	//#define CONFIG_NET_RMII_CLK_EXTERNAL //use external 50MHz clock source
	#define CONFIG_AML_ETHERNET    1                   /*to link /driver/net/aml_ethernet.c*/
	#define IP101PHY    1                   /*to link /driver/net/aml_ethernet.c*/
	#define CONFIG_HOSTNAME        arm_m8
	#define CONFIG_ETHADDR         00:15:18:01:81:31   /* Ethernet address */
	#define CONFIG_IPADDR          10.18.9.97          /* Our ip address */
	#define CONFIG_GATEWAYIP       10.18.9.1           /* Our getway ip address */
	#define CONFIG_SERVERIP        10.18.9.113         /* Tftp server ip address */
	#define CONFIG_NETMASK         255.255.255.0
#endif /* (CONFIG_CMD_NET) */

//I2C definitions
#define CONFIG_AML_I2C			1
#ifdef CONFIG_AML_I2C
#define CONFIG_CMD_I2C			1
#define HAS_AO_MODULE
#define CONFIG_SYS_I2C_SPEED	400000
#endif	//#ifdef CONFIG_AML_I2C

#define CONFIG_CMD_AML
/*
 * PMU definitions, all PMU devices must be include involved
 * in CONFIG_PLATFORM_HAS_PMU
 */
#define CONFIG_PLATFORM_HAS_PMU
#ifdef CONFIG_PLATFORM_HAS_PMU

#define CONFIG_RN5T618
#ifdef CONFIG_RN5T618
#define CONFIG_ALWAYS_POWER_ON                      // if platform without battery, must have
#define CONFIG_DISABLE_POWER_KEY_OFF                // disable power off PMU by long press power key

/*
 * under some cases default voltage of PMU output is
 * not suitable for application, so you should take care
 * of the following macros which defined initial voltage
 * of each power domain when in SPL stage of uboot.
 */
#define CONFIG_VCCK_VOLTAGE             1100        // CPU core voltage when boot, must have
#define CONFIG_VDDAO_VOLTAGE            1150        // VDDAO voltage when boot, must have
#define CONFIG_DDR_VOLTAGE              1500        // DDR voltage when boot, must have

#define CONFIG_VDDIO_AO28               2900        // VDDIO_AO28 voltage when boot, option
#define CONFIG_VDDIO_AO18               1800        // VDDIO_AO18 voltage when boot, option
#define CONFIG_RTC_0V9                   900        // RTC_0V9 voltage when boot, option
#define CONFIG_VDD_LDO                  2700        // VDD_LDO voltage when boot, option
#define CONFIG_VCC1V8                   1800        // VCC1.8v voltage when boot, option
#define CONFIG_VCC2V8                   2850        // VCC2.8v voltage when boot, option
#define CONFIG_AVDD1V8                  1800        // AVDD1.8V voltage when boot, option

/*
 * set to 1 if you want decrease voltage of VDDAO when suspend
 */
#define CONFIG_VDDAO_VOLTAGE_CHANGE     1
#ifdef CONFIG_VDDAO_VOLTAGE_CHANGE
#define CONFIG_VDDAO_SUSPEND_VOLTAGE    825         // voltage of VDDAO when suspend
#endif /* CONFIG_VDDAO_VOLTAGE_CHANGE */

/*
 * DCDC mode switch when suspend
 */
#define CONFIG_DCDC_PFM_PMW_SWITCH      1
#endif /* CONFIG_RN5T618 */

#endif /* CONFIG_PLATFORM_HAS_PMU */

#define CONFIG_SDIO_B1   1
#define CONFIG_SDIO_A    1
#define CONFIG_SDIO_B    1
#define CONFIG_SDIO_C    1
#define CONFIG_ENABLE_EXT_DEVICE_RETRY 1


#define CONFIG_MMU                    1
#define CONFIG_PAGE_OFFSET 	0xc0000000
//#define CONFIG_SYS_LONGHELP	1

/* USB
 * Enable CONFIG_MUSB_HCD for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_UDD for Device functionalities.
 */
/* #define CONFIG_MUSB_UDC		1 */
#define CONFIG_CMD_USB 1
#if defined(CONFIG_CMD_USB)
	#define CONFIG_M8_USBPORT_BASE_A	0xC9040000
	#define CONFIG_M8_USBPORT_BASE_B	0xC90C0000
	#define CONFIG_USB_STORAGE      1
	#define CONFIG_USB_DWC_OTG_HCD  1
	#define CONFIG_USB_DWC_OTG_294	1
#endif //#if defined(CONFIG_CMD_USB)



//#define CONFIG_PREBOOT "mw da004004 80000510;mw c81000014 4000;mw c1109900 0"

#define CONFIG_CMD_AUTOSCRIPT

#define CONFIG_CMD_REBOOT 1
#define CONFIG_PREBOOT


/* Environment information */
#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTFILE		boot.img

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x12000000\0" \
	"loadaddr_logo=0x13000000\0" \
	"testaddr=0x12400000\0" \
	"console=ttyS0,115200n8\0" \
	"bootm_low=0x00000000\0" \
	"bootm_size=0x80000000\0" \
	"mmcargs=setenv bootargs console=${console} " \
	"boardname=m8_board\0" \
	"chipname=8726m8\0" \
	"get_dt=checkhw\0" \
	"initrd_high=60000000\0" \
	"hdmimode=1080p\0" \
	"cvbsmode=576cvbs\0" \
	"outputmode=1080p\0" \
	"vdac_config=0x10\0" \
	"initargs=root=/dev/system rootfstype=ext4 init=/init console=ttyS0,115200n8 no_console_suspend\0" \
	"video_dev=tvout\0" \
	"display_width=1920\0" \
	"display_height=1080\0" \
	"display_bpp=16\0" \
	"display_color_format_index=16\0" \
	"display_layer=osd2\0" \
	"display_color_fg=0xffff\0" \
	"display_color_bg=0\0" \
	"fb_addr=0x15100000\0" \
	"fb_width=1280\0"\
	"fb_height=720\0"\
	"partnum=2\0" \
	"p0start=1000000\0" \
	"p0size=400000\0" \
	"p0path=uImage\0" \
	"p1start=1400000\0" \
	"p1size=8000000\0" \
	"p1path=android.rootfs\0" \
	"bootstart=0\0" \
	"bootsize=100000\0" \
	"bootpath=u-boot.bin\0" \
	"sdcburncfg=aml_sdc_burn.ini\0"\
	"normalstart=1000000\0" \
	"normalsize=400000\0" \
	"firstboot=1\0" \
	"store=0\0"\
    \
    "preboot="\
        "echo preboot ...; "\
        "run storeargs;"\
        "get_rebootmode; clear_rebootmode; echo reboot_mode=${reboot_mode}\0" \
    \
	"storeargs="\
        "setenv bootargs ${initargs} vdaccfg=${vdac_config} logo=osd1,loaded,${fb_addr},${outputmode},full hdmimode=${hdmimode} cvbsmode=${cvbsmode} androidboot.firstboot=${firstboot} hdmitx=${hdmimode}\0"\
    \
    "bootsdargs="\
        "root=/dev/mmcblk0p1 rw rootfstype=vfat rootwait init=/init console=ttyS0,115200n8 no_console_suspend logo=osd1,loaded,${fb_addr},${outputmode},full hdmimode=${hdmimode} cvbsmode=${cvbsmode} hdmitx=${hdmimode}\0"\
    \
    "bootupdateargs="\
        "root=/dev/mmcblk0p1 rw rootfstype=vfat rootwait init=/init console=ttyS0,115200n8 no_console_suspend logo=osd1,loaded,${fb_addr},${outputmode},full hdmimode=${hdmimode} cvbsmode=${cvbsmode} hdmitx=${hdmimode} firmware=rootfs.tar.gz\0"\
	\
     "bootsdcard="\
        "echo Booting ...;"\
         "setenv bootargs ${bootsdargs}; "\
         "mmcinfo;"\
         "fatload mmc 0 ${loadaddr} boot.img;"\
         "bootm\0" \
     "bootupdate="\
        "echo Updating...;"\
         "setenv bootargs ${bootupdateargs}; "\
         "mmcinfo;"\
         "fatload mmc 0 ${loadaddr} boot.img;"\
         "bootm\0" \
    \
	"recovery="\
        "echo enter recovery;"\
        "if mmcinfo; then "\
            "if fatload mmc 0 ${loadaddr} recovery.img; then bootm;fi;"\
        "fi; "\
    \
    "sdc_burning=sdc_burn ${sdcburncfg}\0"


#define CONFIG_BOOTCOMMAND   "run bootsdcard"

#define CONFIG_AUTO_COMPLETE	1
#define CONFIG_ENV_SIZE         (64*1024)



#define CONFIG_MMC_BOOT

#ifdef CONFIG_MMC_BOOT
#define CONFIG_ENV_OVERWRITE


//emmc

#define CONFIG_ENV_IN_EMMC_OFFSET 0x80000


	#define CONFIG_ENV_IS_IN_MMC
	#define CONFIG_CMD_SAVEENV
    #define CONFIG_SYS_MMC_ENV_DEV 1
	#define CONFIG_ENV_OFFSET       0x1000000
#endif


//----------------------------------------------------------------------
//Please set the M8 CPU clock(unit: MHz)
//legal value: 600, 792, 996, 1200,1440
#define M8_CPU_CLK 		    (1200)
#define CONFIG_SYS_CPU_CLK	(M8_CPU_CLK)
//----------------------------------------------------------------------

//-----------------------------------------------------------------------
//DDR setting
#define CONFIG_M8_DDR_CHANNEL_SET (CONFIG_M8_DDRX2_S12)
#define CONFIG_M8_DDR_AMBM_SET    (CONFIG_M8_DDR_ADDR_MAP_BANK_MODE_4_BNK)


//For M8 DDR clock gating disable
#define CONFIG_M8_GATEACDDRCLK_DISABLE 1


//For M8 DDR PUB WL/WD/RD/RG-LVT, WD/RD-BVT disable
#define CONFIG_M8_PUB_WLWDRDRGLVTWDRDBVT_DISABLE 1

//Please just define m8 DDR clock here only
//current DDR clock range (408~804)MHz with fixed step 12MHz
#define CFG_M8_DDR_CLK    792 //696 //768  //792// (636)

//On board DDR capactiy
#define CFG_M8_DDR3_1GB
//#define CFG_M8_DDR3_2GB
//#define CFG_M8_DDR3_4GB
//above setting will affect following:
//board/amlogic/m8_k200_v1/firmware/timming.c
//arch/arm/cpu/aml_meson/m8/mmutable.s

//DDR row/col size
//row size.  2'b01 : A0~A12.   2'b10 : A0~A13.  2'b11 : A0~A14.  2'b00 : A0~A15.
//col size.   2'b01 : A0~A8,      2'b10 : A0~A9
#if   defined(CFG_M8_DDR3_1GB)
	//2Gb(X16) x 4pcs
	#define CONFIG_M8_DDR3_ROW_SIZE (2)
	#define CONFIG_M8_DDR3_COL_SIZE (2)
	#define CONFIG_M8_DDR_ROW_BITS  (14)
#elif defined(CFG_M8_DDR3_2GB)
	//4Gb(X16) x 4pcs
	#define CONFIG_M8_DDR3_ROW_SIZE (3)
	#define CONFIG_M8_DDR3_COL_SIZE (2)
	#define CONFIG_M8_DDR_ROW_BITS  (15)
#elif defined(CFG_M8_DDR3_4GB)
	//8Gb(X16) x 4pcs
	#define CONFIG_M8_DDR3_ROW_SIZE (0)
	#define CONFIG_M8_DDR3_COL_SIZE (2)
	#define CONFIG_M8_DDR_ROW_BITS  (16)
#elif !defined(CFG_M8_DDR3_1GB) && !defined(CFG_M8_DDR3_2GB) && !defined(CFG_M8_DDR3_4GB)
	#error "Please set DDR3 capacity first in file m8_k200_v1.h\n"
#endif


/*if board need use ddr scramble function, please enable DDR_SCRAMBE_ENABLE
 * the function do testing below :
 * 1  performance test:   if enable the function, performance don't have obvious differenc
 * 2  ICE connect testing: ICE can connect board in uboot and kernel, when exception is occured, ICE can connect board in uboot and kernel also
 * 3  suspend test: if enable the function, suspend/resume is ok
 * 4  ddr scramble key is random
**/
//#define DDR_SCRAMBE_ENABLE  1

#define CONFIG_DUMP_DDR_INFO 1


#define CONFIG_ENABLE_WRITE_LEVELING 1

#define PHYS_MEMORY_START        (0x00000000) // ???
#if defined(CFG_M8_DDR3_1GB)
	#define PHYS_MEMORY_SIZE     (0x40000000) // 1GB
#elif defined(CFG_M8_DDR3_2GB)
	#define PHYS_MEMORY_SIZE     (0x80000000) // 2GB
#elif defined(CFG_M8_DDR3_4GB)
	#define PHYS_MEMORY_SIZE     (0x100000000) // 4GB ??
#else
	#error "Please define DDR3 memory capacity in file m8_k200_v1.h\n"
#endif

#define CONFIG_SYS_MEMTEST_START      0x10000000  /* memtest works on */
#define CONFIG_SYS_MEMTEST_END        0x18000000  /* 0 ... 128 MB in DRAM */
//#define CONFIG_ENABLE_MEM_DEVICE_TEST 1
#define CONFIG_NR_DRAM_BANKS	      1	          /* CS1 may or may not be populated */

/* Pass open firmware flat tree*/
#define CONFIG_OF_LIBFDT	1
#define CONFIG_SYS_BOOTMAPSZ   PHYS_MEMORY_SIZE       /* Initial Memory map for Linux */
#define CONFIG_ANDROID_IMG	1

#define CONFIG_CMD_IMGPACK 1

/*-----------------------------------------------------------------------
 * power down
 */
//#define CONFIG_CMD_SUSPEND 1
#define CONFIG_CMD_LOGO


/*
* CPU switch test for uboot
*/
//#define CONFIG_M8_TEST_CPU_SWITCH 1


#endif //__CONFIG_M8_K200_V1_H__
